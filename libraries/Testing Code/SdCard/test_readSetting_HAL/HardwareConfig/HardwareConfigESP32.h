/*
*****************
MPU 9250 / MS5611
*****************
 
SDA_PIN 	IO21
SCL_PIN 	IO22

*******************
Ecran EPaper 1.54''
*******************

CS/SS 		IO5
DC 				IO17
RST 			IO16
BUSY 			IO4

MOSI			15
MISO			2
SCK				14

****
LED
**** 

LED_BUILTIN 	22

*******
SDCard
*******

mySD.h

M5Stack.h

CS 			13
MOSI		15
MISO		2
SCK			14

*********
Boutons
*********

M4Stack.h

Bouton A	38
Bouton B	37
Bouton C	39

************
Speaker
************

PWM/DAC		IO25

 NS4148, http://www.chipsourcetek.com/Uploads/file/20151207193630_0605.pdf amplificateur audio mono 3W - Classe D connecté sur la pin GPIO 25

(XT_DAC_Audio) for ESP32  http://www.xtronical.com/the-dacaudio-library-download-and-installation/ 

PWM/DAC	IO26		optional ampli class D
Enable	IO34	enable optional ampli class D

******************
GPS
******************

RX	IO19

******************************
Lecture de la charge batterie
******************************

tension  IO35	

            vbat
             |
            100k
             |
      IO35 ---
             |
            100k
             |
            GND

*/


#ifndef _HARDWARECONFIGESP32_H_
#define _HARDWARECONFIGESP32_H_

#if defined(ESP32)

/******************************/
/*            SCREEN          */
/******************************/

/* Set the pins used for Screen modules */

// pins_arduino.h, e.g. LOLIN32
//static const uint8_t SS    = 5;
//static const uint8_t MOSI  = 23;
//static const uint8_t MISO  = 19;
//static const uint8_t SCK   = 18;

//GxIO_Class io(SPI, SS, 17, 16);
//GxEPD_Class display(io, 16, 4);

#define VARIOSCREEN_CS_PIN SS
#define VARIOSCREEN_DC_PIN 17
#define VARIOSCREEN_RST_PIN 16
#define VARIOSCREEN_BUSY_PIN 4



/****************************/
/*           SDCARD         */
/****************************/

#define SDCARD_CS_PIN   13
#define SDCARD_MOSI_PIN 15
#define SDCARD_MISO_PIN 2
#define SDCARD_SCK_PIN  14

/***************************/
/*        LED              */
/***************************/

#define pinLED (22)

#define LED_ON() 		   {GPIO.out_w1ts = (1 << pinLED);}
#define LED_OFF()		   {GPIO.out_w1tc = (1 << pinLED);}

/****************************/
/*           Buttons        */
/****************************/

#define VARIOBTN_LEFT_PIN 38
#define VARIOBTN_CENTER_PIN 37
#define VARIOBTN_RIGHT_PIN 39

/***********************/
/* The voltage divisor */
/***********************/

#define VOLTAGE_DIVISOR_PIN 35

/*************************/
/*        GPS            */
/*************************/

#define pinGpsTXD  (-1)
#define pinGpsRXD  (19)
#define pinGpsRTS  (-1)
#define pinGpsCTS  (-1)

#define GPS_UART_NUM          UART_NUM_1
//#define UART_RX_BUFFER_SIZE   512
#define UART_RX_BUFFER_SIZE   256

/*************************/
/*         AUDIO         */
/*************************/

#define pinAudioAmpEna       (34)
#define pinAudioDAC          (25)
#define SPEAKER_PIN 25		//or 26
#define TONE_PIN_CHANNEL 0	// or 1

/****************/
/*     TENSION  */
/****************/

#define VOLTAGE_DIVISOR_PIN 35

#endif
#endif