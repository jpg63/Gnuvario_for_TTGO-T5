// Playing a simple melody using the XTronical DAC Audio library
// Demonstrates use of the music score object
// See www.xtronical.com for write ups on sound and for hardware required

//*****************************
// DEBBUGAGE                  *
//*****************************
#include <DebugConfig.h>
#include <VarioLog.h>

#ifdef PROG_DEBUG
#define ARDUINOTRACE_ENABLE 1
#else
#define ARDUINOTRACE_ENABLE 0
#endif

#ifndef ARDUINOTRACE_SERIAL
#define ARDUINOTRACE_SERIAL SerialPort
#endif

#include <ArduinoTrace.h>

#include "MusicDefinitions.h"
#include "XT_DAC_Audio.h"

// Data for the melody. Note followed by optional change in playing length in 1/4 beats. See documentation for more details
int8_t PROGMEM TwinkleTwinkle[] = {
/*  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
  NOTE_SILENCE,BEAT_5,SCORE_END*/
  NOTE_A5,BEAT_2,SCORE_END
};

XT_DAC_Audio_Class DacAudio(25,0);                                            // Create the main player class object. Use GPIO 25 (DAC pin) and timer 0
XT_MusicScore_Class Music(TwinkleTwinkle,TEMPO_ADENTE,INSTRUMENT_ORGAN);     // Music score object, handles tunes. Pass Music Data,Tempo, Instrument 
                                                                              // You can just pass the music data and the rest will default to... well.... default values!

XT_Instrument_Class Bip(INSTRUMENT_ORGAN);

#define PIN_AUDIO_AMP_ENA     19      //Enabled ampli class D

void setup() {

#if defined(ENABLE_DEBUG)
  SerialPort.begin(115200);

  // Wait for USB Serial
  while (!SerialPort)
  {
//    SysCall::yield();
  }
#endif 

  delay(5000);
  pinMode(PIN_AUDIO_AMP_ENA,OUTPUT);
  digitalWrite(PIN_AUDIO_AMP_ENA,HIGH);

  Bip.SetFrequency(2000);
  Bip.SetDuration(500);
/*  Bip.Note=NOTE_C7;      // convert the negative value to positive index.
  Bip.Duration=(((60/float(TEMPO_ADENTE))*BytesPerSec)*(float(NOTE_C7)/4));
      // Then times by 0.8 to allow for natural movement
      // of players finger on the instrument.
  Bip.SoundDuration=Bip.Duration*0.8;
  Bip.Init();*/

//  DacAudio.Play(&Music);  
  DacAudio.Play(&Bip);         
  DacAudio.DacVolume=100;
}

int frequence = 200;

void loop() {
  DacAudio.FillBuffer();          // This needs only be in your main loop once, suggest here at the top.
  // put whatever code you want here that you would normally have on your loop

  if(Bip.Playing==false)    { // if not playing, play again
//    DacAudio.Play(&Music);  
//    delay(200);  
    Bip.SetFrequency(frequence);
    Bip.SetDuration(10);
    frequence += 200;
    if (frequence > 2000) frequence = 20;
    DacAudio.Play(&Bip);     
  }    
}
