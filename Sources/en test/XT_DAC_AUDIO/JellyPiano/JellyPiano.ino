#include "MusicDefinitions.h";
#include "XT_DAC_Audio.h";

const uint8_t THRESHOLD = 15;

XT_DAC_Audio_Class DacAudio(DAC1, 0);

int8_t SCORE_C5[] = { NOTE_C5, SCORE_END };
int8_t SCORE_D5[] = { NOTE_D5, SCORE_END };
int8_t SCORE_E5[] = { NOTE_E5, SCORE_END };
int8_t SCORE_F5[] = { NOTE_F5, SCORE_END };
int8_t SCORE_G5[] = { NOTE_G5, SCORE_END };
int8_t SCORE_A5[] = { NOTE_A5, SCORE_END };
int8_t SCORE_B5[] = { NOTE_B5, SCORE_END };
int8_t SCORE_C6[] = { NOTE_C6, SCORE_END };

XT_MusicScore_Class C5_NOTE(SCORE_C5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class D5_NOTE(SCORE_D5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class E5_NOTE(SCORE_E5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class F5_NOTE(SCORE_F5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class G5_NOTE(SCORE_G5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class A5_NOTE(SCORE_A5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class B5_NOTE(SCORE_B5, TEMPO_ALLEGRO,INSTRUMENT_PIANO);
XT_MusicScore_Class C6_NOTE(SCORE_C6, TEMPO_ALLEGRO,INSTRUMENT_PIANO);

#define PIN_AUDIO_AMP_ENA     19      //Enabled ampli class D

void setup() {
  Serial.begin(115200);

  pinMode(PIN_AUDIO_AMP_ENA,OUTPUT);
  digitalWrite(PIN_AUDIO_AMP_ENA,HIGH);
}

void loop() {
  DacAudio.FillBuffer();
  play_note(T0, C5_NOTE);
  play_note(T3, D5_NOTE);
  play_note(T4, E5_NOTE);
  play_note(T5, F5_NOTE);
  play_note(T6, G5_NOTE);
  play_note(T7, A5_NOTE);
  play_note(T8, B5_NOTE);
  play_note(T9, C6_NOTE);
}

void play_note(uint16_t pin, XT_MusicScore_Class& note) {
  if (note.Playing == false) {
    DacAudio.Play(&note);
    delay(100);
  }
}
