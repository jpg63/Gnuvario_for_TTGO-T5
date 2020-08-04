// Playing a simple melody using the XTronical DAC Audio library
// Demonstrates use of the music score object
// See www.xtronical.com for write ups on sound and for hardware required

//https://github.com/WeekendWarrior1/XTronical_XT_DAC_Audio_Mirror

#include "MusicDefinitions.h"
#include "XT_DAC_Audio2.h"

#define PIN_AUDIO_AMP_ENA     19      //Enabled ampli class D

// Data for the melody. Note followed by optional change in playing length in 1/4 beats. See documentation for more details
int16_t PROGMEM TwinkleTwinkle[] = { 
    2000, SCORE_END 
//  NOTE_C5,SCORE_END
  
  /*NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_G5,NOTE_G5,NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,BEAT_2,
  NOTE_C5,NOTE_C5,NOTE_G5,NOTE_G5,NOTE_A5,NOTE_A5,NOTE_G5,BEAT_2,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_E5,NOTE_D5,NOTE_D5,NOTE_C5,BEAT_4,  
  NOTE_SILENCE,BEAT_5,SCORE_END*/
};

//int8_t note_a_jouer[] = { NOTE_F1,NOTE_F2,FNOTE_F3,FNOTE_F4,FNOTE_F5,FNOTE_F6 };

XT_DAC_Audio_Class DacAudio(25,2);                                            // Create the main player class object. Use GPIO 25 (DAC pin) and timer 0
//XT_MusicScore_Class 
XT_Beeper_Class Music(TwinkleTwinkle,TEMPO_ALLEGRO,INSTRUMENT_ORGAN); // TEMPO_ALLEGRO,INSTRUMENT_ORGAN);     // Music score object, handles tunes. Pass Music Data,Tempo, Instrument 
                                                                              // You can just pass the music data and the rest will default to... well.... default values!
int i = 0;

void setup() {
  pinMode(PIN_AUDIO_AMP_ENA,OUTPUT);
  digitalWrite(PIN_AUDIO_AMP_ENA,HIGH);
  delay(5000);

  DacAudio.Play(&Music);           
}

void loop() {
  DacAudio.FillBuffer();          // This needs only be in your main loop once, suggest here at the top.
  // put whatever code you want here that you would normally have on your loop
  if (Music.Playing == false) {
    i++;
    if (i > 30) {
      i = 0;
  //    Music.Tempo=TEMPO_ALLEGRO;
    }
    TwinkleTwinkle[0] = 200*i; //note_a_jouer[i];
//    Music.Tempo=10*i;
    DacAudio.Play(&Music);           
    delay(100);
  }
}




/*
 void loop() { // Generate a Sine wave
  for (int deg = 0; deg < 360; deg = deg + 8){
    dacWrite(25, int(128 + 80 * (sin(deg*PI/180)+sin(3*deg*PI/180)/3+sin(5*deg*PI/180)/5+sin(7*deg*PI/180)/7+sin(9*deg*PI/180)/9+sin(11*deg*PI/180)/11))); // Square
  }
}

// ESP32 has two 8-bit DAC (digital to analog converter) channels, connected to GPIO25 (Channel 1) and GPIO26 (Channel 2)
// Square wave   = amplitude . sin(x) + sin(3.x) / 3 +  sin (5.x) / 5 + sin (7.x) / 7  + sin (9.x) / 9  + sin (11.x) / 11  Odd harmonics
// Triangle wave = amplitude . sin(x) + 1/3^2.sin(3.x) +  1/5^2.sin(5.x) + 1/7^2.sin (7.x) + 1/9^2.sin(9.x) + 1/11^2.sin (11.x) Odd harmonics
// dacWrite(25, int(128 + 80 * (sin(deg*PI/180)))); // GPIO Pin mode (OUTPUT) is set by the dacWrite function
// dacWrite(25, int(128 + 80 * (sin(deg*PI/180)+sin(3*deg*PI/180)/3+sin(5*deg*PI/180)/5+sin(7*deg*PI/180)/7+sin(9*deg*PI/180)/9+sin(11*deg*PI/180)/11))); // Square
// dacWrite(25, int(128 + 80 * (sin(deg*PI/180)+1/pow(3,2)*sin(3*deg*PI/180)+1/pow(5,2)*sin(5*deg*PI/180)+1/pow(7,2)*sin(7*deg*PI/180)+1/pow(9,2)*sin(9*deg*PI/180))));
 */
