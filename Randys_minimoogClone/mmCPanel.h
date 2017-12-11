//  This is the Front Panel Code for my
//  first iteration of the MinimoogClone Synthesizer
//  Based on Marshall Taylor's (SFE) Proto-8 Synthesizer design
//  Randy Parks  12/11/2017

//Header
#ifndef MMCPANEL_H_INCLUDED
#define MMCPANEL_H_INCLUDED

#include "Panel.h"
//#include "PanelComponents.h"
//#include "HardwareInterfaces.h"
#include <Arduino.h>
//#include "proto-8PanelComponents.h"
#include "proto-8HardwareInterfaces.h"
#include <Audio.h>

///////////////////////////////////
extern AudioSynthWaveform       waveform1;
extern AudioSynthWaveform       waveform2;
extern AudioSynthWaveform       waveform3;
extern AudioMixer4              mixer1;
extern AudioEffectEnvelope      envelope1;


extern uint8_t oscModFlg;
extern uint8_t oscSelVal;
extern uint8_t osc1RangeVal;
extern uint8_t osc2RangeVal;
extern uint8_t osc3RangeVal;
extern uint8_t oscRangeVal;
extern uint8_t osc1WaveVal;
extern uint8_t osc2WaveVal;
extern uint8_t osc3WaveVal;
extern uint8_t oscWaveVal;
extern uint8_t oscModFlg;
extern uint8_t osc3CntrlFlg;
extern uint8_t fltModFlg;
extern uint8_t fltKybdcnt1Flg;
extern uint8_t fltKybdcnt2Flg;
extern uint8_t vmixOsc1Flg;
extern uint8_t vmixOsc2Flg;
extern uint8_t vmixOsc3Flg;
extern float vmixOsc1Val;
extern float vmixOsc2Val;
extern float vmixOsc3Val;
extern float vmixModNoiseVal;
extern float ctlTuneVal;
extern float ctlGlideVal;
extern float ctlMixVal;
extern float fltCutoffVal;
extern float fltEmphasisVal;
extern float lcAttackVal;
extern float lcHoldVal;
extern float lcDecayVal;
extern float lcSustainVal;
extern float noiseFreqVal;
extern float lcReleaseVal;
extern uint8_t vmixExtinFlg;
extern uint8_t vmixNoiseFlg;
extern uint8_t noiSelectFlg;
extern uint8_t vmixA440Flg;
extern uint8_t mvolOutputFlg;

//void displayRangeLEDs();

enum PStates
{
	PInit,
	PRun,
  PDisplay
};


class mmCPanel : public Panel
{
public:
	mmCPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	
public:
	//Internal Panel Components
	Simple10BitKnob ctlTune;
	Simple10BitKnob ctlGlide;
	Simple10BitKnob ctlMix;
	
	Simple10BitKnob fltCutoff;
	Simple10BitKnob fltEmphasis;
	Simple10BitKnob fltContour;
	Simple10BitKnob fltAttack;
	Simple10BitKnob fltDecay;
	Simple10BitKnob fltSustain;
	
	Simple10BitKnob lcAttack;
  Simple10BitKnob lcHold;
	Simple10BitKnob lcDecay;
	Simple10BitKnob lcSustain;
  Simple10BitKnob lcRelease;
	
	Simple10BitKnob osc1Freq;
	Simple10BitKnob osc2Freq;
	Simple10BitKnob osc3Freq;;
	
	Simple10BitKnob noiseFreq;
	
	Simple10BitKnob mvolOutput;
	
	Simple10BitKnob vmixOsc1;
	Simple10BitKnob vmixOsc2;
	Simple10BitKnob vmixOsc3;
	Simple10BitKnob vmixModNoise;
	Simple10BitKnob vmixExtin;
  
	Button oscSelect;
	Button oscRange;
	Button oscWave;
	Button oscMod;
	Button osc3Cntrl;
	
	Button fltMod;
	Button fltKybdcnt1;
	Button fltKybdcnt2;

	Button vmixOsc1Sw;
	Button vmixOsc2Sw;
	Button vmixOsc3Sw;
	Button vmixExtinSw;
	Button vmixNoiseSw;
	
	Button noiSelect;
	Button vmixA440Sw;
	Button mvolOutputSw;


public:
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
  Led osc1MixOn;
  Led osc2MixOn;
  Led osc3MixOn;
  Led mixA440On;
  Led mvolOutputOn;
  Led kybdGateOn;
 
private:	
	Led fltModOn;
	Led fltKybdcnt1On;
	Led fltKybdcnt2On;


	//State machine stuff  
	PStates state;

};


#endif
