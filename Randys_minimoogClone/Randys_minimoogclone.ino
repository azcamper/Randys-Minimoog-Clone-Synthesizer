//  This is my first iteration of the MinimoogClone Synthesizer
//  Based on Marshall Taylor's (SFE) Proto-8 Synthesizer design
//  Randy Parks  12/11/2017

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "SerialFlash.h"
#include "proto-8Hardware.h"
#include "mmCPanel.h"
#include "note_values.h"

/*
// GUItool: begin automatically generated code
AudioSynthWaveform       waveform3;      //xy=203,723
AudioSynthWaveform       waveform2;      //xy=231,598
AudioSynthWaveform       waveform1;      //xy=404,509
AudioSynthNoiseWhite     noise1;         //xy=406,817
AudioSynthNoisePink      pink1;          //xy=406,889
AudioSynthWaveformSineModulated sine_fm1;       //xy=415,962
AudioEffectMultiply      multiply1;      //xy=523,723
AudioMixer4              mixer2;         //xy=749,795
AudioMixer4              mixer1;         //xy=752,604
AudioFilterStateVariable filter1;        //xy=1003,885
AudioFilterStateVariable filter2;        //
AudioEffectEnvelope      envelope1;      //xy=1048,682
AudioOutputI2S           i2s1;           //xy=1234,688
AudioConnection          patchCord1(waveform3, 0, mixer1, 2);
AudioConnection          patchCord2(waveform3, 0, multiply1, 1);
AudioConnection          patchCord3(waveform3, sine_fm1);
AudioConnection          patchCord4(waveform2, 0, mixer1, 1);
AudioConnection          patchCord5(waveform2, 0, multiply1, 0);
AudioConnection          patchCord6(waveform1, 0, mixer1, 0);
AudioConnection          patchCord7(noise1, 0, filter2, 0);
AudioConnection          patchCord8(pink1, 0, mixer2, 1);
AudioConnection          patchCord9(sine_fm1, 0, mixer2, 3);
AudioConnection          patchCord10(multiply1, 0, mixer2, 2);
AudioConnection          patchCord11(mixer2, 0, mixer1, 3);
AudioConnection          patchCord12(mixer1, 0, filter1, 0);
AudioConnection          patchCord13(filter1, 0, envelope1, 0);
AudioConnection          patchCord14(envelope1, 0, i2s1, 0);
AudioConnection          patchCord15(envelope1, 0, i2s1, 1);
AudioConnection          patchCord16(filter2, 2, mixer2, 0);

AudioControlSGTL5000     sgtl5000_1;     //xy=128,1132
// GUItool: end automatically generated code
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveform       waveform3;      //xy=123,706
AudioSynthWaveform       waveform2;      //xy=128,585
AudioSynthWaveform       waveform1;      //xy=134,468
AudioSynthNoiseWhite     noise1;         //xy=328,795
AudioSynthWaveformSineModulated sine_fm1;       //xy=443,1051
AudioSynthNoisePink      pink1;          //xy=450,952
AudioFilterStateVariable filter2;        //xy=521,796
AudioEffectMultiply      multiply1;      //xy=523,723
AudioMixer4              mixer2;         //xy=749,795
AudioMixer4              mixer1;         //xy=852,545
AudioFilterStateVariable filter1;        //xy=955,745
AudioEffectEnvelope      envelope1;      //xy=1112,629
AudioOutputI2S           i2s1;           //xy=1324,628
AudioConnection          patchCord1(waveform3, 0, mixer1, 2);
AudioConnection          patchCord2(waveform3, 0, multiply1, 1);
AudioConnection          patchCord3(waveform3, sine_fm1);
AudioConnection          patchCord4(waveform2, 0, mixer1, 1);
AudioConnection          patchCord5(waveform2, 0, multiply1, 0);
AudioConnection          patchCord6(waveform1, 0, mixer1, 0);
AudioConnection          patchCord7(noise1, 0, filter2, 0);
AudioConnection          patchCord8(sine_fm1, 0, mixer2, 3);
AudioConnection          patchCord9(pink1, 0, mixer2, 1);
AudioConnection          patchCord10(filter2, 0, mixer2, 0);
AudioConnection          patchCord11(multiply1, 0, mixer2, 2);
AudioConnection          patchCord12(mixer2, 0, mixer1, 3);
AudioConnection          patchCord13(mixer1, 0, filter1, 0);
AudioConnection          patchCord14(filter1, 0, envelope1, 0);
AudioConnection          patchCord15(envelope1, 0, i2s1, 0);
AudioConnection          patchCord16(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=128,1132
// GUItool: end automatically generated code


//**Timers and stuff**************************//
#include "timerModule32.h"
//Globals
uint32_t maxTimer = 60000000;
uint32_t maxInterval = 2000000;

#include "timeKeeper.h"
//**Panels and stuff**************************//

// **Timers and stuff**************************//
IntervalTimer myTimer;

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set maxInterval to the max foreseen interval of any TimerClass
//  Set maxTimer to overflow number in the header.  maxTimer + maxInterval
//    cannot exceed variable size.

TimerClass32 midiRecordTimer( 1000 );
//TimerClass32 panelUpdateTimer(30000);
//TimerClass32 routerUpdateTimer(30000);
//TimerClass32 envTimer( 200 );

uint8_t debugLedStates = 1;

TimerClass32 LEDsTimer(20);
TimerClass32 switchesTimer(500);
TimerClass32 knobsTimer(500);
TimerClass32 serialTimer( 500000 ); // 0.5 seconds
TimerClass32 panelTimer( 5000 ); //5ms
TimerClass32 debugTimer(1000000);

//tick variable for interrupt driven timer1
uint32_t usTicks = 0;
uint8_t usTicksMutex = 1; //start locked out

mmCPanel myCustomPanel;

//Names used in P8PanelComponents.cpp and .h

LEDShiftRegister LEDs;
AnalogMuxTree knobs;
SwitchMatrix switches;

// MIDI things
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiA);

//  Panel flags

uint8_t oscSelVal = 1;  // Osc selected 1,2 0r 3
uint8_t osc1RangeVal = 3;  //middle octave
uint8_t osc2RangeVal = 3;  //middle octave
uint8_t osc3RangeVal = 3;  //middle octave
uint8_t oscRangeVal = 3;
uint8_t osc1WaveVal = 7;
uint8_t osc2WaveVal = 7;
uint8_t osc3WaveVal = 7;
uint8_t oscWaveVal = 7;
uint8_t oscModFlg = 0;
uint8_t osc3CntrlFlg = 0;
uint8_t fltModFlg = 0;
uint8_t fltKybdcnt1Flg = 0;
uint8_t fltKybdcnt2Flg = 0;
uint8_t vmixOsc1Flg = 0;
uint8_t vmixOsc2Flg = 0;
uint8_t vmixOsc3Flg = 0;
float vmixOsc1Val = 0.0;
float vmixOsc2Val = 0.0;
float vmixOsc3Val = 0.0;
float ctlTuneVal;
float ctlGlideVal;
float ctlMixVal;
float vmixModNoiseVal = 0.0; // Modulation/Noise mixer volume
float fltCutoffVal;
float fltEmphasisVal;
float lcAttackVal;
float lcHoldVal;
float lcDecayVal;
float lcSustainVal;
float lcReleaseVal;
float noiseFreqVal;
uint8_t vmixExtinFlg = 0;
uint8_t vmixNoiseFlg = 0;
uint8_t noiSelectFlg = 0;
uint8_t vmixA440Flg = 0;
uint8_t mvolOutputFlg = 1;

uint8_t usTicksLocked = 1; //start locked out


uint8_t lastAttack = 10;
int8_t lastAttackBend = 0;
uint8_t lastDecay = 10;
int8_t lastDecayBend = 0;
uint8_t lastRelease = 10;
int8_t lastReleaseBend = 0;

#include "note_values.h"


void HandleNoteOn(byte channel, byte pitch, byte velocity)
{
 if (vmixA440Flg == 0) { //Don't do a key on if A440 flag
	Serial.print("Note On :"); 
	Serial.print("0x"); 
	Serial.print(channel, HEX); 
	Serial.print(" 0x"); 
	Serial.print(pitch, HEX);
	Serial.print(" 0x"); 
  Serial.print(pitch);
	Serial.print(velocity, HEX); 
	Serial.print("\n"); 
	myCustomPanel.kybdGateOn.setState(LEDON);
/*        Serial.print("OSC Select Value = ");
        Serial.println(oscSelVal);
        Serial.print("\n");
        Serial.print("oscRange Value = ");
        Serial.println(oscRangeVal);
        Serial.print("osc 1 Vmix Value = ");
        Serial.println(vmixOsc1Val);
        Serial.print("osc 2 Vmix Value = ");
        Serial.println(vmixOsc2Val);
        Serial.print("osc 3 Vmix Value = ");
        Serial.println(vmixOsc3Val);
*/
               switch (osc1WaveVal) {
                  case 1: waveform1.begin(WAVEFORM_TRIANGLE);
                  break;
                  case 2: waveform1.begin(WAVEFORM_SAWTOOTH_REVERSE);
                  break;
                  case 3: waveform1.begin(WAVEFORM_SAWTOOTH);
                  break;
                  case 4: waveform1.begin(WAVEFORM_SQUARE);
                  break;
                  case 5: waveform1.begin(WAVEFORM_PULSE);
                          waveform1.pulseWidth(0.2);
                  break;
                  case 6: waveform1.begin(WAVEFORM_PULSE);
                          waveform1.pulseWidth(0.8);
                  break;
                  case 7: waveform1.begin(WAVEFORM_SINE);
                  break;
               }
               switch (osc2WaveVal) {
                  case 1: waveform2.begin(WAVEFORM_TRIANGLE);
                  break;
                  case 2: waveform2.begin(WAVEFORM_SAWTOOTH_REVERSE);
                  break;
                  case 3: waveform2.begin(WAVEFORM_SAWTOOTH);
                  break;
                  case 4: waveform2.begin(WAVEFORM_SQUARE);
                  break;
                  case 5: waveform2.begin(WAVEFORM_PULSE);
                  break;
                  case 6: waveform2.begin(WAVEFORM_PULSE);
                  break;
                  case 7: waveform2.begin(WAVEFORM_SINE);
                  break;
               }
               switch (osc3WaveVal) {
                  case 1: waveform3.begin(WAVEFORM_TRIANGLE);
                  break;
                  case 2: waveform3.begin(WAVEFORM_SAWTOOTH_REVERSE);
                  break;
                  case 3: waveform3.begin(WAVEFORM_SAWTOOTH);
                  break;
                  case 4: waveform3.begin(WAVEFORM_SQUARE);
                  break;
                  case 5: waveform3.begin(WAVEFORM_PULSE);
                          waveform3.pulseWidth(0.9);
                  break;
                  case 6: waveform3.begin(WAVEFORM_PULSE);
                          waveform3.pulseWidth(0.1);
                  break;
                  case 7: waveform3.begin(WAVEFORM_SINE);
                  break;
               }
  

 // SETUP ENVELOPE
 // Max delays are 200 mS
  envelope1.attack(lcAttackVal * 200);
  envelope1.hold(lcHoldVal * 200);
  envelope1.decay(lcDecayVal * 200);
  envelope1.sustain(lcSustainVal * 200);
  envelope1.release(lcReleaseVal * 200);
  envelope1.noteOn();  // Start the envelope

	if (oscRangeVal > 0) {    
	//Don't change freq on Osc 1 & 2 if osc Range is LO
  // Service Osc 1 & Osc 2
	  for(byte x = 48 ; x < 73; x++){
        if (x == pitch) {
           sine_fm1.frequency(note_frequency[(x - 36) + ((osc2RangeVal-1) * 24)]);
           Serial.print("sine_fm1 freq = ");
           Serial.println(note_frequency[(x - 36) + ((osc2RangeVal-1) * 24)]);
			     if (vmixOsc1Flg == 1) {
        	      waveform1.frequency(note_frequency[(x - 36) + ((osc1RangeVal-1) * 24)]);
                Serial.println(note_frequency[(x - 36) + ((osc1RangeVal-1) * 24)]);
				        mixer1.gain(0, vmixOsc1Val);
			     }
           if (vmixOsc2Flg == 1) {
			          waveform2.frequency(note_frequency[(x - 36) + ((osc2RangeVal-1) * 24)]);
                Serial.println(note_frequency[(x - 36) + ((osc2RangeVal-1) * 24)]);
                mixer1.gain(1, vmixOsc2Val);
           }

         if (oscModFlg == 0) {  // no modulation
		      if (vmixOsc3Flg == 1) {
            if(osc3RangeVal == 0) {
			         waveform3.amplitude(0.75);
               waveform3.frequency(note_frequency[(x - 36) + ((osc3RangeVal) * 24)]);
               Serial.println(note_frequency[(x - 36) + ((osc3RangeVal) * 24)]);
               mixer1.gain(2, vmixOsc3Val);
              }
            else
              {
               waveform3.frequency(note_frequency[(x - 48) + ((osc3RangeVal-1) * 24)]);
               Serial.println(note_frequency[(x - 48) + ((osc3RangeVal-1) * 24)]);
               mixer1.gain(2, vmixOsc3Val);
              }
            }
		   }
		   else  // oscillator 3 is a modulator
		   {
        Serial.println("Osc 3 is a modulator");
        mixer1.gain(3, vmixModNoiseVal);
		    waveform3.frequency(ctlGlideVal);
			  waveform3.amplitude(ctlMixVal);
			  mixer2.gain (0, 0.0);
			  mixer2.gain (1, 0.0);
		 if (osc3CntrlFlg == 0) { // frequency Mod
        sine_fm1.amplitude(0.50);
			  mixer2.gain (2, 0.0);
			  mixer2.gain (3, 0.50);
			}
			else  // Amplitude Mod
			{
			  mixer2.gain (2, 0.50);
			  mixer2.gain (3, 0.0);
			}
 		   }
           break;  //done looking for pitch 
       }
	   }
	 }

	if ((vmixNoiseFlg == 1) & (oscModFlg == 0)) {
               mixer1.gain(3, vmixModNoiseVal);
               if (noiSelectFlg == 0) {
                  mixer2.gain(0, 0.0); // Pink Noise
                  mixer2.gain(1, 0.50);
               }
               else
               {
                 // Noise Freq Knob adjusts Filter2 Freq Cutoff
                 // formula from Teensy Audio Workshop Part 2-07 filters
                  filter2.frequency(noiseFreqVal * 1200);
                  filter2.resonance(5);
                  mixer2.gain(0, 0.50); // White Noise
                  mixer2.gain(1, 0.0);
               }
           }

// Apply Low Pass Filter

  // formula from Teensy Audio Workshop Part 2-07 filters
  filter1.frequency(expf((float)fltCutoffVal / 150.0) * 10.0 + 80.0);
  filter1.resonance(fltEmphasisVal * 5);

 }
}

void HandleNoteOff(byte channel, byte pitch, byte velocity)
{
 if (vmixA440Flg == 0) { //Don't do a key on if A440 flag
	Serial.print("Note Off:"); 
	Serial.print("0x"); 
	Serial.print(channel, HEX); 
	Serial.print(" 0x"); 
	Serial.print(pitch, HEX); 
	Serial.print(" 0x"); 
	Serial.print(velocity, HEX); 
	Serial.print("\n");

	myCustomPanel.kybdGateOn.setState(LEDOFF);
  envelope1.noteOff();  // Start the envelope release
  delay(lcReleaseVal * 250);  // wait for Release delay
 
	mixer1.gain(0, 0.0); // !Turn Off Osc 1
  Serial.print( "A440 Flag = ");
  Serial.println(vmixA440Flg);
	mixer1.gain(1, 0.0); // !Turn Off Osc 2
  mixer1.gain(2, 0.0); // !Turn Off Osc 3
  mixer1.gain(3, 0.0); // !Turn Off Noise

 }
}



void HandleControlChange(byte channel, byte number, byte value)
{
	Serial.print(number);
	Serial.print(", ");
	Serial.println(value);

	switch( number )
	{
		case 1:
		//Filter
//		filter1.frequency((value * value * 0.9) + 40);  
		break;
		default:
		break;
	}
}

void handleSystemReset(void)
{
  myCustomPanel.kybdGateOn.setState(LEDOFF);
}

void serviceUS(void)
{
  uint32_t returnVar = 0;
  if(usTicks >= ( maxTimer + maxInterval ))
  {
    returnVar = usTicks - maxTimer;
  }
  else
  {
    returnVar = usTicks + 1;
  }
  usTicks = returnVar;
  usTicksLocked = 0;  //unlock
}

uint32_t tapTempoTimerMath( uint16_t BPMInput )
{
	uint32_t returnVar = 0;
	
	returnVar = 2500000 /( (uint32_t)BPMInput );
	return returnVar;
}


// -----------------------------------------------------------------------------
void setup() 
{
	//Initialize serial:
	Serial.begin(9600);
	//delay(2000);
	Serial.println("Program Started");
	
  // initialize IntervalTimer
  myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.000001 seconds
  myCustomPanel.reset();

	LEDs.begin();
	knobs.begin();
	switches.begin();
  
	//Connect MIDI handlers
	midiA.setHandleNoteOn(HandleNoteOn);  // Put only the name of the function
	midiA.setHandleNoteOff(HandleNoteOff);
	midiA.setHandleControlChange(HandleControlChange);
	midiA.setHandleSystemReset(handleSystemReset);
	// Initiate MIDI communications, listen to all channels
	midiA.begin(MIDI_CHANNEL_OMNI);
	Serial.println("Starting program"); 

	//midiA.turnThruOn();
	midiA.turnThruOff();


  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.32);
  
  mixer1.gain(0, 0.0);
  mixer1.gain(1, 0.0);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  mixer2.gain(0, 0.50);
  mixer2.gain(1, 0.50);
  mixer2.gain(2, 0.50);
  mixer2.gain(3, 0.0);
  waveform1.begin(WAVEFORM_SINE);
  waveform1.amplitude(0.75);
  waveform1.frequency(261.63);
  waveform1.pulseWidth(0.50);
  waveform2.begin(WAVEFORM_SINE);
  waveform2.amplitude(0.75);
  waveform2.frequency(261.63);
  waveform2.pulseWidth(0.50);
  waveform3.begin(WAVEFORM_SINE);
  waveform3.amplitude(0.75);
  waveform3.frequency(261.63);
  waveform3.pulseWidth(0.50);

  pink1.amplitude(0.75);
  noise1.amplitude(0.75);
  envelope1.attack(10);
  envelope1.hold(10);
  envelope1.decay(25);
  envelope1.sustain(0.4);
  envelope1.release(70);

// int waveform_type = WAVEFORM_SINE;
// int mixer1_setting = 0;
// int mixer2_setting = 0;
// elapsedMillis timeout = 0;
// bool mixer2_envelope = false;
	
}

void loop()
{

//**Copy to make a new timer******************//  

	//Update the timers, but only once per interrupt
	if( usTicksLocked == 0 )
	{
		//**Copy to make a new timer******************//  
		//msTimerA.update(usTicks);
		
		debugTimer.update(usTicks);
		serialTimer.update(usTicks);
		panelTimer.update(usTicks);
		LEDsTimer.update(usTicks);
		switchesTimer.update(usTicks);
		knobsTimer.update(usTicks);
		
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.
	
	//**Copy to make a new timer******************//  
	//  if(msTimerA.flagStatus() == PENDING)
	//  {
	//    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
	//  }
	
	if(debugTimer.flagStatus() == PENDING)
	{
		//User code
//		digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	if(serialTimer.flagStatus() == PENDING)
	{
	   //User code
	  // Serial.println("Hello Randy");
	}
    if(panelTimer.flagStatus() == PENDING)
    {
     	myCustomPanel.tickStateMachine(5);
    }
    
	if(LEDsTimer.flagStatus() == PENDING)
    {
		LEDs.send();
		//Serial.println("LEDs Timer Pending");
    }

    if(switchesTimer.flagStatus() == PENDING)
    {
		switches.scan();
    }

    if(knobsTimer.flagStatus() == PENDING)
    {
		knobs.scan();
    }


	midiA.read();
}


