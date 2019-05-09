/****************************************************/
/*                                                  */
/*          Libraries ToneDAC ESP32                 */
/*                                                  */
/*  version    Date     Description                 */
/*    1.0    07/03/19                               */
/*                                                  */
/****************************************************/

#if defined(ESP32)


#include "toneDAC_esp32.h"
#include <Arduino.h>
#include "DebugConfig.h"
#include "HardwareConfig.h"
//#include "utility\DebugConfig.h"
//#include "utility\Config.h"

/*#include "ConfigESP32.h"
#include "ConfigMK0.h"
#include "ConfigPRO.h"*/

#include "soc/rtc_io_reg.h"
#include "soc/rtc_cntl_reg.h"
#include "soc/sens_reg.h"
#include "soc/rtc.h"

#include "driver/dac.h"

/*// BEEP PIN
#if not defined(_CONFIG_H_)
#define SPEAKER_PIN 26		//or 25
#endif*/

#if not defined(_DEBUG_H_)
#define TONEDAC_DEBUG					//debug Tone
//#define SerialPort Serial
#endif

static int clk_8m_div = 7;  // RTC 8M clock divider (division is by clk_8m_div+1, i.e. 0 means 8MHz frequency)
static int frequency_step = 8;  // Frequency step for CW generator
static int scale = 0;           // full scale
static int offset;              // leave it default / 0 = no any offset
static int invert = 2;          // invert MSB to get sine waveform

#define AUDIO_AMP_ENABLE()   {GPIO.out1_w1ts.val = ((uint32_t)1 << (pinAudioAmpEna - 32));}
#define AUDIO_AMP_DISABLE()  {GPIO.out1_w1tc.val = ((uint32_t)1 << (pinAudioAmpEna - 32));}

		// BEEP PIN
uint8_t Speaker_Pin;	 

#if defined(TONEDAC_EXTENDED)
#endif //TONEDAC_EXTENDED

/***********************************/
void ToneDacEsp32::dac_cosine_enable(dac_channel_t channel) {
/***********************************/
	// Enable tone generator common to both channels
	SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
	channelDAC = channel;
	switch(channel) {
		case DAC_CHANNEL_1:
		// Enable / connect tone tone generator on / to this channel
		SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
		// Invert MSB, otherwise part of waveform will have inverted
		SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, 2, SENS_DAC_INV1_S);
		break;

		case DAC_CHANNEL_2:
		SET_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
		SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, 2, SENS_DAC_INV2_S);
		break;
		default :
#ifdef TONEDAC_DEBUG	
		SerialPort.print("Channel : ");
		SerialPort.println(channel);
#endif TONEDAC_DEBUG
	}
}


 // Set frequency of internal CW generator common to both DAC channels 
 // clk_8m_div: 0b000 - 0b111
 // frequency_step: range 0x0001 - 0xFFFF
 
/***********************************/
void ToneDacEsp32::dac_frequency_set(int clk_8m_div, int frequency_step) {
/***********************************/
	REG_SET_FIELD(RTC_CNTL_CLK_CONF_REG, RTC_CNTL_CK8M_DIV_SEL, clk_8m_div);
  SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL1_REG, SENS_SW_FSTEP, frequency_step, SENS_SW_FSTEP_S);
}


 // Scale output of a DAC channel using two bit pattern:
 // 00: no scale
 // 01: scale to 1/2
 // 10: scale to 1/4
 // 11: scale to 1/8

/***********************************/
void ToneDacEsp32::dac_scale_set(dac_channel_t channel, int scale) {
/***********************************/
	switch(channel) {
		case DAC_CHANNEL_1:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE1, scale, SENS_DAC_SCALE1_S);
				break;
		case DAC_CHANNEL_2:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_SCALE2, scale, SENS_DAC_SCALE2_S);
				break;
		default : break;
	}
}


// Offset output of a DAC channel
// Range 0x00 - 0xFF
/***********************************/
void ToneDacEsp32::dac_offset_set(dac_channel_t channel, int offset) {
/***********************************/
	switch(channel) {
		case DAC_CHANNEL_1:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC1, offset, SENS_DAC_DC1_S);
				break;
		case DAC_CHANNEL_2:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_DC2, offset, SENS_DAC_DC2_S);
				break;
		default : break;
	}
}


 // Invert output pattern of a DAC channel
 // 00: does not invert any bits,
 // 01: inverts all bits,
 // 10: inverts MSB,
 // 11: inverts all bits except for MSB
/***********************************/
void ToneDacEsp32::dac_invert_set(dac_channel_t channel, int invert) {
/***********************************/
	switch(channel) {
		case DAC_CHANNEL_1:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV1, invert, SENS_DAC_INV1_S);
				break;
		case DAC_CHANNEL_2:
				SET_PERI_REG_BITS(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_INV2, invert, SENS_DAC_INV2_S);
				break;
		default : break;
	}
}


/***********************************/
void ToneDacEsp32::init(void) {
/***********************************/
//	ledcSetup(TONE_PIN_CHANNEL, 0, 13);
//  ledcAttachPin(SPEAKER_PIN, TONE_PIN_CHANNEL);

  init(SPEAKER_PIN);
  
}

/***********************************/
void ToneDacEsp32::init(uint32_t pin) {
/***********************************/

  if ((pin != 25) or (pin != 26)) pin = 25;
  Speaker_Pin = pin;
	
	if (Speaker_Pin == 25) channelDAC = DAC_CHANNEL_1;
	else									 channelDAC = DAC_CHANNEL_2;
	
	AUDIO_AMP_DISABLE();
  dac_cosine_enable(channelDAC);
  dac_output_enable(channelDAC);
	scale =  3; //3 - opt.misc.speakerVolume;
	offset = 0;
	invert = 2;
  dac_scale_set(channelDAC, scale);
  dac_offset_set(channelDAC, offset);
  dac_invert_set(channelDAC, invert);
}

/***********************************/
void ToneDacEsp32::begin(uint32_t srate) {
/***********************************/
/*#if defined(TONEDAC_EXTENDED)
	audioFileReady = false;
#endif //TONEDAC_EXTENDED

  sampleRate = srate;
	waveForm = WAVEFORM_WAV;
  sIndex = 0;   //Set to zero to start from beginning of waveform
	
	/*Modules configuration *
//  digitalWrite(Speaker_Pin, 0);

	tcConfigure(srate);*/
}

/***********************************/
void ToneDacEsp32::tone(unsigned long frequency
/***********************************/
#ifdef TONEDAC_VOLUME
            , uint8_t volume
#endif
#ifdef TONEDAC_LENGTH
            , unsigned long length, uint8_t background
#endif
	    ) {

  /* check if no tone */ 
  if (toneDACMuted || frequency == 0
#ifdef TONEDAC_VOLUME     
      || volume == 0
#endif
      ) { noTone(); return; } 

#ifdef TONEDAC_DEBUG	
	SerialPort.print("volume toneDacEsp32 = ");
	SerialPort.println(volume);
#endif TONEDAC_DEBUG
			
  /* check volume */
#ifdef TONEDAC_VOLUME
  if (volume > 100) volume = 100;
	if (_tDAC_volume != volume) 
	  {
	    _tDAC_volume = volume;
			uint8_t scaleVolume;
			if (volume <= 25) scaleVolume = 0;
			else if ((volume > 25) && (volume <= 50)) scaleVolume = 1;
			else if ((volume > 50) && (volume <= 72)) scaleVolume = 2;
			else scaleVolume = 3;
		}
#endif

  if (frequency > 10000) frequency = 10000;
	_frequency = frequency;
	
/*	AUDIO_AMP_DISABLE();
  dac_cosine_enable(channelDAC);
  dac_output_enable(channelDAC);
	scale =  2; //3 - opt.misc.speakerVolume;
	offset = 0;
	invert = 2;
	dac_scale_set(channelDAC, 1); //3 - scale);
  dac_offset_set(channelDAC, offset);
  dac_invert_set(channelDAC, invert);*/

	frequency_step = (frequency*(1+clk_8m_div)*65536)/RTC_FAST_CLK_FREQ_APPROX;
  dac_frequency_set(clk_8m_div, frequency_step);
  AUDIO_AMP_ENABLE();

	#ifdef TONEDAC_LENGTH
  if (length > 0 && background) {  // Background tone playing, returns control to your sketch.

    _tDAC_time = millis() + length; // Set when the note should end.
    //TODO !!!                     // Activate the timer interrupt.
  }

  if (length > 0 && !background) { delay(length); noToneDAC(); } // Just a simple delay, doesn't return control untill finished.
#endif
}

/***********************************/
void ToneDacEsp32::noTone() {
/***********************************/
	dac_output_disable(channelDAC);
  AUDIO_AMP_DISABLE();
}

#ifdef TONEDAC_LENGTH
// TODO !!! { // Timer interrupt vector.
//  if (millis() >= _tAC_time) noToneAC(); // Check to see if it's time for the note to end.
//}
#endif

/***********************************/
void ToneDacEsp32::toneMute(bool newMuteState) {
/***********************************/
  /* stop tone if needed */
  if( newMuteState ) {
    noTone();
  }

  /* save */
  toneDACMuted = newMuteState;
}


#if defined(TONEDAC_EXTENDED)


#endif //TONEDAC_EXTENDED

/***********************************/
uint8_t ToneDacEsp32::getVolume()
/***********************************/
{
  return _tDAC_volume;
}

#endif //EPS32
