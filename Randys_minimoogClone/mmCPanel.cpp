//  This is the Front Panel Code for my
//  first iteration of the MinimoogClone Synthesizer
//  Based on Marshall Taylor's (SFE) Proto-8 Synthesizer design
//  Randy Parks  12/11/2017


#include "Panel.h"
#include "mmCPanel.h"
//#include "panelComponents.h"
//#include "HardwareInterfaces.h"
#include <Arduino.h>
#include "proto-8PanelComponents.h"
#include "proto-8HardwareInterfaces.h"
#include "proto-8Hardware.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

extern AudioControlSGTL5000 sgtl5000_1;

  Led oscModOn;
  Led osc3Cntr1On;
  Led osc3Cntr2On;
  Led osc1SelectOn;
  Led osc2SelectOn;
  Led osc3SelectOn;
  Led oscRangeLoOn;
  Led oscRange32On;
  Led oscRange16On;
  Led oscRange8On;
  Led oscRange4On;
  Led oscRange2On;
  Led oscWaveTri;
  Led oscWaveRsw;
  Led oscWaveSaw;
  Led oscWaveSqu;
  Led oscWavePl1;
  Led oscWavePl2;
  Led oscWaveSine;
  Led kybdGateOn;
  Led noiseOn;
  Led noiseWhiteOn;
  Led noisePinkOn;
  Led mixA440On;

LEDShiftRegister PanelLEDs;  
mmCPanel::mmCPanel( void )
{
//  Knobs

// CONTROLLERS KNOBS

	ctlTune.setHardware(new Proto8AnalogIn( 1 )); // TUNE
	add ( &ctlTune );
	ctlGlide.setHardware(new Proto8AnalogIn( 9 )); // GLIDE 
	add ( &ctlGlide );
	ctlMix.setHardware(new Proto8AnalogIn( 10 )); // MOD MIX
	add ( &ctlMix );

// FILTER KNOBS

	fltCutoff.setHardware(new Proto8AnalogIn( 2 )); // CUTOFF
	add ( &fltCutoff );
	fltEmphasis.setHardware(new Proto8AnalogIn( 3 )); // EMPHASIS
	add ( &fltEmphasis );
	fltContour.setHardware(new Proto8AnalogIn( 4 )); // CONTOUR
	add ( &fltContour );
	fltAttack.setHardware(new Proto8AnalogIn( 5 )); // ATTACK
	add ( &fltAttack );
	fltDecay.setHardware(new Proto8AnalogIn( 6 )); // DECAY
	add ( &fltDecay );
	fltSustain.setHardware(new Proto8AnalogIn( 7 )); // SUSTAIN
	add ( &fltSustain );
	
// LOUDNESS CONTOUR KNOBS
	
	lcAttack.setHardware(new Proto8AnalogIn( 8 )); // ATTACK
	add ( &lcAttack );
  lcHold.setHardware(new Proto8AnalogIn( 17 )); // ATTACK
  add ( &lcHold );
	lcDecay.setHardware(new Proto8AnalogIn( 18 ));// DECAY
	add ( &lcDecay );
	lcSustain.setHardware(new Proto8AnalogIn( 19 ));// SUSTAIN
	add ( &lcSustain );
  lcRelease.setHardware(new Proto8AnalogIn( 20 )); // ATTACK
  add ( &lcRelease );

// OSCILLATORS FREQUENCY KNOBS
	
	osc1Freq.setHardware(new Proto8AnalogIn( 11 )); // OSC 1 FREQ
	add ( &osc1Freq );
	osc2Freq.setHardware(new Proto8AnalogIn( 12 )); // OSC 2 FREQ
	add ( &osc2Freq );
	osc3Freq.setHardware(new Proto8AnalogIn( 13 )); // OSC 3 FREQ
	add ( &osc3Freq );

// NOISE FREQUENCY KNOB
	
	noiseFreq.setHardware(new Proto8AnalogIn( 14 )); // NOISE FREQ
	add ( &noiseFreq );
	
// MAIN OUTPUT VOLUME KNOB
	
	mvolOutput.setHardware(new Proto8AnalogIn( 21 )); // MAIN OUTPUT VOLUME
	add ( &mvolOutput );

// VOLUME MIXER KNOBS
	
	vmixOsc1.setHardware(new Proto8AnalogIn( 25 )); // OSC 1 VOLUME
	add ( &vmixOsc1 );
	vmixOsc2.setHardware(new Proto8AnalogIn( 26 )); // OSC 2 VOLUME
	add ( &vmixOsc2 );
	vmixOsc3.setHardware(new Proto8AnalogIn( 27 )); // OSC 3 VOLUME
	add ( &vmixOsc3 );
	vmixModNoise.setHardware(new Proto8AnalogIn( 28 )); // OSC NOISE VOLUME
	add ( &vmixModNoise );
	vmixExtin.setHardware(new Proto8AnalogIn( 29 )); // EXT IN VOLUME
	add ( &vmixExtin );

	
	// Buttons

// OSCILLATOR BUTTONS
	
	oscSelect.setHardware(new Proto8DigitalIn( 1,1 ), 0);  // OSC SELECT
	add ( &oscSelect );
	oscRange.setHardware(new Proto8DigitalIn( 2,1 ), 0);  // OSC RANGE
	add ( &oscRange );
	oscWave.setHardware(new Proto8DigitalIn( 3,1 ), 0);  // OSC WAVEFORM
	add ( &oscWave );
	oscMod.setHardware(new Proto8DigitalIn( 1,2 ), 0);  // OSC MOD
	add ( &oscMod );
	osc3Cntrl.setHardware(new Proto8DigitalIn( 2,2 ), 0);  // OSC3 CONTROL
	add ( &osc3Cntrl );

// FILTER BUTTONS

	fltMod.setHardware(new Proto8DigitalIn( 6,1 ), 0);  // FILTER MOD
	add ( &fltMod );
	fltKybdcnt1.setHardware(new Proto8DigitalIn( 7,1 ), 0);  // KEYBOARD CONTROL 1
	add ( &fltKybdcnt1 );
	fltKybdcnt2.setHardware(new Proto8DigitalIn( 8,1 ), 0);  // KEYBOARD CONTROL 2
	add ( &fltKybdcnt2 );

// VOLUME MIXER BUTTONS

	vmixOsc1Sw.setHardware(new Proto8DigitalIn( 1,4 ), 0);  // OSC 1 ON/OFF SWITCH
	add ( &vmixOsc1Sw );
	vmixOsc2Sw.setHardware(new Proto8DigitalIn( 2,4 ), 0);  // OSC 2 ON/OFF SWITCH
	add ( &vmixOsc2Sw );
	vmixOsc3Sw.setHardware(new Proto8DigitalIn( 3,4 ), 0);  // OSC 3 ON/OFF SWITCH
	add ( &vmixOsc3Sw );
	vmixExtinSw.setHardware(new Proto8DigitalIn( 4,4 ), 0);  // EXT IN ON/OFF SWITCH
	add ( &vmixExtinSw );
	vmixNoiseSw.setHardware(new Proto8DigitalIn( 5,4 ), 0);  // NOISE ON/OFF SWITCH
	add ( &vmixNoiseSw );

// NOISE SELECT BUTTON

	noiSelect.setHardware(new Proto8DigitalIn( 6,4 ), 0);  // NOISE SELECT
	add ( &noiSelect );

// A-440 ON/OFF and MAIN OUTPUT VOLUME ON/OFF BUTTONS

	vmixA440Sw.setHardware(new Proto8DigitalIn( 7,4 ), 0);  // A-440 ON/OFF SWITCH
	add ( &vmixA440Sw );
	mvolOutputSw.setHardware(new Proto8DigitalIn( 8,4 ), 0);  // MAIN VOLUME ON/OFF SWITCH
	add ( &mvolOutputSw );
	

   // LEDs
   
// OSCILLATOR LED's   

	oscModOn.setHardware(new Proto8DigitalOut( 1 ), 0);  // OSC MOD ON
	add ( &oscModOn );
	osc3Cntr1On.setHardware(new Proto8DigitalOut( 2 ), 0);  // OSC 3 CONTROL 1 ON
	add ( &osc3Cntr1On );
	osc3Cntr2On.setHardware(new Proto8DigitalOut( 3 ), 0);  // OSC 3 CONTROL 2 ON
	add ( &osc3Cntr2On );
	osc1SelectOn.setHardware(new Proto8DigitalOut( 17 ), 0);  // OSC 1 SELECT ON
	add ( &osc1SelectOn );
	osc2SelectOn.setHardware(new Proto8DigitalOut( 18 ), 0);  // OSC 2 SELECT ON
	add ( &osc2SelectOn );
	osc3SelectOn.setHardware(new Proto8DigitalOut( 19 ), 0);  // OSC 3 SELECT ON
	add ( &osc3SelectOn );
	oscRangeLoOn.setHardware(new Proto8DigitalOut( 20 ), 0);  // OSC RANGE LO ON
	add ( &oscRangeLoOn );
	oscRange32On.setHardware(new Proto8DigitalOut( 21 ), 0);  // OSC RANGE 32 ON
	add ( &oscRange32On );
	oscRange16On.setHardware(new Proto8DigitalOut( 22 ), 0);  // OSC RANGE 16 ON
	add ( &oscRange16On );
	oscRange8On.setHardware(new Proto8DigitalOut( 23 ), 0);  // OSC RANGE 8 ON
	add ( &oscRange8On );
	oscRange4On.setHardware(new Proto8DigitalOut( 24 ), 0);  // OSC RANGE 4 ON
	add ( &oscRange4On );
	oscRange2On.setHardware(new Proto8DigitalOut( 33 ), 0);  // OSC RANGE 2 ON
	add ( &oscRange2On );
	oscWaveTri.setHardware(new Proto8DigitalOut( 26 ), 0);  // OSC WAVEFORM TRIANGLE
	add ( &oscWaveTri );
	oscWaveRsw.setHardware(new Proto8DigitalOut( 27 ), 0);  // OSC WAVEFORM REVERSE SAWTOOTH
	add ( &oscWaveRsw );
	oscWaveSaw.setHardware(new Proto8DigitalOut( 28 ), 0);  // OSC WAVEFORM SAWTOOTH
	add ( &oscWaveSaw );
	oscWaveSqu.setHardware(new Proto8DigitalOut( 29 ), 0);  // OSC WAVEFORM SQUARE
	add ( &oscWaveSqu );
	oscWavePl1.setHardware(new Proto8DigitalOut( 34 ), 0);  // OSC WAVEFORM PULSE 1
	add ( &oscWavePl1 );
	oscWavePl2.setHardware(new Proto8DigitalOut( 31 ), 0);  // OSC WAVEFORM PULSE 2
	add ( &oscWavePl2 );
	oscWaveSine.setHardware(new Proto8DigitalOut( 32 ), 0);  // OSC WAVEFORM Sine
	add ( &oscWaveSine );

// KEYBOARD GATE

	kybdGateOn.setHardware(new Proto8DigitalOut( 64 ), 0);  // KEYBOARD GATE ON
	add ( &kybdGateOn );
	
// FILTER LED's	

	fltModOn.setHardware(new Proto8DigitalOut( 14 ), 0);  // FILTER MOD ON
	add ( &fltModOn );
	fltKybdcnt1On.setHardware(new Proto8DigitalOut( 15 ), 0);  // KEYBOARD CONTROL 1 ON
	add ( &fltKybdcnt1On );
	fltKybdcnt2On.setHardware(new Proto8DigitalOut( 16 ), 0);  // KEYBOARD CONTROL 2 ON
	add ( &fltKybdcnt2On );

// VOLUME MIXER LED's  

  osc1MixOn.setHardware(new Proto8DigitalOut( 49 ), 0);  // OSC 1 MIX ON
  add ( &osc1MixOn );
  osc2MixOn.setHardware(new Proto8DigitalOut( 50 ), 0);  // OSC 2 MIX ON
  add ( &osc2MixOn );
  osc3MixOn.setHardware(new Proto8DigitalOut( 51 ), 0);  // OSC 3 MIX ON
  add ( &osc3MixOn );
  mixA440On.setHardware(new Proto8DigitalOut( 61 ), 0);  // A440 ON
  add ( &mixA440On );
  mvolOutputOn.setHardware(new Proto8DigitalOut( 62 ), 0);  // MAIN OUTPUT ON
  add ( &mvolOutputOn );


// NOISE LED's  

  noiseOn.setHardware(new Proto8DigitalOut( 53 ), 0);  // MIX NOISE ON
  add ( &noiseOn );
  noiseWhiteOn.setHardware(new Proto8DigitalOut( 55 ), 0);  // MIX NOISE ON
  add ( &noiseWhiteOn );
  noisePinkOn.setHardware(new Proto8DigitalOut( 54 ), 0);  // MIX NOISE ON
  add ( &noisePinkOn );


	state = PInit;
}


void mmCPanel::reset( void )
{
	state = PInit;
}


void mmCPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );

	// ***** PROCESS THE LOGIC ***** //
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		nextState = PRun;
		break;
	case PRun:
      if( oscSelect.serviceRisingEdge() )
       {
         Serial.print(" Osc Sel Value = ");
         Serial.print(oscSelVal);

		   if (oscSelVal == 3) {
			   oscSelVal = 1;
		   }
		   else
		   {
			   oscSelVal++;
		   }
		 }	
 
       if( oscRange.serviceRisingEdge() )
        {
         Serial.println("Osc Range Button Pressed");
         Serial.print(" Osc Sel Value = ");
         Serial.print(oscSelVal);
         
        switch (oscSelVal) {
           case 1:
                if (osc1RangeVal < 6) {
                   osc1RangeVal++ ;
			   Serial.print(" Osc 1 Range Value = ");
				   Serial.print(osc1RangeVal);
                 }
                else
                {
                   osc1RangeVal = 1 ;
                }
                break;
           case 2:
                if (osc2RangeVal < 6) {
                   osc2RangeVal++ ;
                }
                else
                {
                   osc2RangeVal = 1 ;
                }
                break;
           case 3:
                if (osc3RangeVal < 6) {
                osc3RangeVal++ ;
                }
                else
                {
                osc3RangeVal = 1 ;
                }
                break;
       }
		}	   

       if( oscWave.serviceRisingEdge() )
        {
         Serial.println("Osc Wave Button Pressed");
         Serial.print(" Osc Sel Value = ");
         Serial.print(oscSelVal);
         
        switch (oscSelVal) {
           case 1:
                if (osc1WaveVal < 7) {
                   osc1WaveVal++ ;
         Serial.print(" Osc 1 Wave Value = ");
           Serial.print(osc1WaveVal);
                 }
                else
                {
                   osc1WaveVal = 1 ;
                }
                break;
           case 2:
                if (osc2WaveVal < 7) {
                   osc2WaveVal++ ;
                }
                else
                {
                   osc2WaveVal = 1 ;
                }
                break;
           case 3:
                if (osc3WaveVal < 7) {
                osc3WaveVal++ ;
                }
                else
                {
                osc3WaveVal = 1 ;
                }
                break;
       }
    }
    
// Oscillator Modulation Select		
      if( oscMod.serviceRisingEdge() )
       {
        if (oscModFlg == 0) {
          oscModOn.setState(LEDON);
          oscModFlg = 1;
          if (osc3CntrlFlg  == 0) {
            osc3Cntr1On.setState(LEDON);
            osc3Cntr2On.setState(LEDOFF);
            }
            else
            { 
            osc3Cntr1On.setState(LEDOFF);
            osc3Cntr2On.setState(LEDON);
            }
        }   
        else
        {
          oscModOn.setState(LEDOFF);
          oscModFlg = 0;
        }
      }
       
	   
// Oscillator 3 Mod Control Select
//     0 = Freq	Mod, 1 = Amp Mod
    if (oscModFlg == 1) {
      if( osc3Cntrl.serviceRisingEdge() )
       {
        if (osc3CntrlFlg  == 0) {
          osc3Cntr1On.setState(LEDOFF);
		      osc3Cntr2On.setState(LEDON);
          osc3CntrlFlg  = 1;
          //Serial.println("Turn ON OSC 3 MOD CONTROL 2 LED ");
          }
        else
          {
          osc3Cntr1On.setState(LEDON);
		      osc3Cntr2On.setState(LEDOFF);
		  //Serial.println("Turn ON OSC 3 MOD CONTROL 1 LED ");
          osc3CntrlFlg  = 0;
         }
       }
     }
    else
     {
       osc3Cntr1On.setState(LEDOFF);
       osc3Cntr2On.setState(LEDOFF);
     }
    
    
// Oscillator 1 MIXER VOLUME ON   
      if( vmixOsc1Sw.serviceRisingEdge() )
       {
        if (vmixOsc1Flg == 0) {
          osc1MixOn.setState(LEDON);
          vmixOsc1Flg = 1;
          //Serial.println("Turn ON OSC 1 MIX LED ");
          }
        else
          {
          osc1MixOn.setState(LEDOFF);
          vmixOsc1Flg = 0;
          }
       }

// Oscillator 2 MIXER VOLUME ON   
      if( vmixOsc2Sw.serviceRisingEdge() )
       {
        if (vmixOsc2Flg == 0) {
          osc2MixOn.setState(LEDON);
          vmixOsc2Flg = 1;
          //Serial.println("Turn ON OSC 2 MIX LED ");
          }
        else
          {
          osc2MixOn.setState(LEDOFF);
          vmixOsc2Flg = 0;
          }
       }

// Oscillator 3 MIXER VOLUME ON   
      if( vmixOsc3Sw.serviceRisingEdge() )
       {
        if (vmixOsc3Flg == 0) {
          osc3MixOn.setState(LEDON);
          vmixOsc3Flg = 1;
          //Serial.println("Turn ON OSC 3 MIX LED ");
          }
        else
          {
          osc3MixOn.setState(LEDOFF);
          vmixOsc3Flg = 0;
          }
       }
	   
// Noise Mixer Volume On		
      if( vmixNoiseSw.serviceRisingEdge() )
       {
        if (vmixNoiseFlg == 0) {
          noiseOn.setState(LEDON);
          vmixNoiseFlg = 1;
          if (noiSelectFlg  == 0) {
            noisePinkOn.setState(LEDON);
            noiseWhiteOn.setState(LEDOFF);
            }
            else
            { 
            noisePinkOn.setState(LEDOFF);
            noiseWhiteOn.setState(LEDON);
            }
        }   
        else
        {
          noiseOn.setState(LEDOFF);
          vmixNoiseFlg = 0;
        }
        if (oscModFlg == 1) {  
          // Turn off these LEDs in Osc Modulation
            noisePinkOn.setState(LEDOFF);
            noiseWhiteOn.setState(LEDOFF);
          }
        }
      	   
// Noise Select On
//     0 = Pink, 1 = White
    if (vmixNoiseFlg == 1) {
      if( noiSelect.serviceRisingEdge() )
       {
        if (noiSelectFlg  == 0) {
          noisePinkOn.setState(LEDOFF);
		      noiseWhiteOn.setState(LEDON);
          noiSelectFlg  = 1;
          //Serial.println("Turn ON OSC 3 MOD CONTROL 2 LED ");
          }
        else
          {
          noisePinkOn.setState(LEDON);
		      noiseWhiteOn.setState(LEDOFF);
		  //Serial.println("Turn ON OSC 3 MOD CONTROL 1 LED ");
          noiSelectFlg  = 0;
         }
       }
     }
    else
     {
       noisePinkOn.setState(LEDOFF);
       noiseWhiteOn.setState(LEDOFF);
     }

// A440 ON   
      if( vmixA440Sw.serviceRisingEdge() )
       {
        if (vmixA440Flg == 0) {
          mixA440On.setState(LEDON);
          vmixA440Flg = 1;
          waveform1.begin(WAVEFORM_SINE);
          waveform1.frequency( 440.0 ); // A440 ON
          waveform1.amplitude( 0.50 );
          mixer1.gain (0, 0.50);
          envelope1.noteOn();  // Start the envelope
          //Serial.println("Turn ON A440 LED ");
          }
        else
          {
          mixA440On.setState(LEDOFF);
          vmixA440Flg = 0;
          mixer1.gain (0, 0.0);  // A440 OFF
          envelope1.noteOn();  // Start the envelope
          }
       }

 // MAIN OUTPUT VOLUME ON   
      if( mvolOutputSw.serviceRisingEdge() )
       {
        if (mvolOutputFlg == 0) {
          mvolOutputOn.setState(LEDON);
          mvolOutputFlg = 1;
          }
        else
          {
          mvolOutputOn.setState(LEDOFF);
          mvolOutputFlg = 0;
          }
       }

	   
//////////  KNOBS
 	   
// MASTER VOLUME  
		if( mvolOutput.serviceChanged() )
		{
			if (mvolOutputFlg == 1) {
			  sgtl5000_1.volume((float)mvolOutput.getState()/1024);
			}
     else
     {
        sgtl5000_1.volume(0.0);  // Shut OFF Main Volume
     }
		}


//OSCILLATOR 1 MIXER VOLUME		
		if( vmixOsc1.serviceChanged() )
		{
      vmixOsc1Val = ((float)vmixOsc1.getState()/1024);
//			Serial.print("knob OSC 1 Volume: ");
//			Serial.println(vmixOsc1Val);
 		}
    
//OSCILLATOR 2 MIXER VOLUME    
    if( vmixOsc2.serviceChanged() )
    {
      vmixOsc2Val = ((float)vmixOsc2.getState()/1024);
//      Serial.print("knob OSC 2 Volume: ");
//      Serial.println(vmixOsc2Val);
    }

//OSCILLATOR 3 MIXER VOLUME    
    if( vmixOsc3.serviceChanged() )
    {
      vmixOsc3Val = ((float)vmixOsc3.getState()/1024);
//      Serial.print("knob OSC 3 Volume: ");
//      Serial.println(vmixOsc3Val);
    }

// NOISE and OSC 3 MOD VOLUME    
    if( vmixModNoise.serviceChanged() )
    {
      vmixModNoiseVal = ((float)vmixModNoise.getState()/1024);
//      Serial.print("knob NOISE Volume: ");
//      Serial.println(vmixModNoiseVal);
    }

// MOD CONTROL GLIDE    
    if( ctlGlide.serviceChanged() )
    {
      ctlGlideVal = ((float)ctlGlide.getState());
//      Serial.print("knob MOD CONTROL GLIDE: ");
//      Serial.println(ctlGlideVal);
    }
	
// MOD CONTROL MIX    
    if( ctlMix.serviceChanged() )
    {
      ctlMixVal = ((float)ctlMix.getState()/1024);
//      Serial.print("knob MOD CONTROL MIX: ");
//      Serial.println(ctlMixVal);
    }
		
// CONTROL TUNE
    if( ctlTune.serviceChanged() )
    {
      ctlTuneVal = ((float)ctlTune.getState()/1024);
//      Serial.print("knob MOD CONTROL TUNE: ");
//      Serial.println(ctlTuneVal);
    }

// FILTER CUTOFF
    if( fltCutoff.serviceChanged() )
    {
      fltCutoffVal = ((float)fltCutoff.getState());
    }

// FILTER EMPHASIS
    if( fltEmphasis.serviceChanged() )
    {
      fltEmphasisVal = ((float)fltEmphasis.getState()/1024);
    }

// LOUDNESS CONTOUR ATTACK
    if( lcAttack.serviceChanged() )
    {
      lcAttackVal = ((float)lcAttack.getState()/1024);
    }

// LOUDNESS CONTOUR HOLD
    if( lcHold.serviceChanged() )
    {
      lcHoldVal = ((float)lcHold.getState()/1024);
    }

// LOUDNESS CONTOUR DECAY
    if( lcDecay.serviceChanged() )
    {
      lcDecayVal = ((float)lcDecay.getState()/1024);
    }

// LOUDNESS CONTOUR SUSTAIN
    if( lcSustain.serviceChanged() )
    {
      lcSustainVal = ((float)lcSustain.getState()/1024);
    }

// LOUDNESS CONTOUR RELEASE
    if( lcRelease.serviceChanged() )
    {
      lcReleaseVal = ((float)lcRelease.getState()/1024);
    }

// NOISE FREQUENCY
    if( noiseFreq.serviceChanged() )
    {
      noiseFreqVal = ((float)noiseFreq.getState()/1024);
    }

    
/// GO TO NEXT STATE		
		nextState = PDisplay;
		break;
		
	case PDisplay:

 		switch (oscSelVal) {
           case 1:
      		    osc3SelectOn.setState(LEDOFF);
              osc1SelectOn.setState(LEDON);
              break;
           case 2:
      		    osc1SelectOn.setState(LEDOFF);
              osc2SelectOn.setState(LEDON);
              break;
           case 3:
      		  osc2SelectOn.setState(LEDOFF);
            osc3SelectOn.setState(LEDON);
              break;
			  }
    
	  oscRangeLoOn.setState(LEDOFF);
    oscRange32On.setState(LEDOFF);
    oscRange16On.setState(LEDOFF);
    oscRange8On.setState(LEDOFF);
    oscRange4On.setState(LEDOFF);
    oscRange2On.setState(LEDOFF);
    oscWaveTri.setState(LEDOFF);
    oscWaveRsw.setState(LEDOFF);
    oscWaveSaw.setState(LEDOFF);
    oscWaveSqu.setState(LEDOFF);
    oscWavePl1.setState(LEDOFF);
    oscWavePl2.setState(LEDOFF);
    oscWaveSine.setState(LEDOFF);


 //   int tempval = 0;
    switch (oscSelVal) {
       case 1: oscRangeVal =  osc1RangeVal;
               oscWaveVal = osc1WaveVal;
               break;
       case 2: oscRangeVal =  osc2RangeVal;
               oscWaveVal = osc2WaveVal;
               break;
       case 3: oscRangeVal =  osc3RangeVal;
               oscWaveVal = osc3WaveVal;
               break;
    }

    switch (oscRangeVal) {
         case 1: oscRangeLoOn.setState(LEDON);
                 break;
         case 2: oscRange32On.setState(LEDON);
                 break;
         case 3: oscRange16On.setState(LEDON);
                 break;
         case 4: oscRange8On.setState(LEDON);
                 break;
         case 5: oscRange4On.setState(LEDON);
                 break;
         case 6: oscRange2On.setState(LEDON);
                 break;
     }	

    switch (oscWaveVal) {
         case 1: oscWaveTri.setState(LEDON);
                 break;
         case 2: oscWaveRsw.setState(LEDON);
                 break;
         case 3: oscWaveSaw.setState(LEDON);
                 break;
         case 4: oscWaveSqu.setState(LEDON);
                 break;
         case 5: oscWavePl1.setState(LEDON);
                 break;
         case 6: oscWavePl2.setState(LEDON);
                 break;
         case 7: oscWaveSine.setState(LEDON);
                 break;

     }  
  
//	default:
		nextState = PInit;
//		break;
	

 }
  state = nextState;

}


 
