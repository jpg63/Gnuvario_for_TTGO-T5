#ifndef PARAM_H_
#define PARAM_H_

// CJMCU-117 MPU9250+MS5611 circuit interface
//
// VCC  VCC
// GND  GND
//
// MS5611 - MPU9250			WIRE/I2C (SERCOM0)
// SCL  D12 		SCL			WIRE/I2C - SERCOM0
// SDA  D11 		SDA			MS5611 / MPU9250
//
// SWITCHS
// 				A1,A2 				Switch
// 				D5    				Switch Droite - Power
//
// RST   	A4		 	Reset
//
// E-Ink								SPI (SERCOM1)
// CS    	D4
// BUSY  	D3
// RST   	A3
// DC    	D7
// DIN   	MOSI/D8
// CLK   	SCK/D9
//
// GPS	FGPMMOPA6C, construite autour du jeu de puces MTK3339		Serial1 (SERCOM5)
// TX    	D14						serial1  
// RX     D13 					Serial1     
//
// Bluetooth / Microship RN4871		SERIAL (SERCOM3)
// TX    	D0 						sercom3
// RX    	D1 						sercom3
// 				A5						RN4871:RST
//				A6						RN4871:INT		sleep / wake manager
// 
// SD-CARD		SPI (SERCOM2)
// MOSI		MOSI					SD SPI
// SCL		SCK						SD SPI
// SS			SS						SD SPI	
// MISO		MISO					SD SPI
// CD			CD						SD SPI
//
// Buzzer
//    		D2						PWM
//
//				A0/DAC0				DAC0
//				D6						Mute Max
//
// ADC_BATTERY					Battery voltage
// LED_BUILTIN					LED

/******************************/
/*            SCREEN          */
/******************************/

/* Set the pins used for Screen modules */

#if defined(ESP8266)

// generic/common.h
//static const uint8_t SS    = 15;
//static const uint8_t MOSI  = 13;
//static const uint8_t MISO  = 12;
//static const uint8_t SCK   = 14;
// pins_arduino.h
//static const uint8_t D8   = 15;
//static const uint8_t D7   = 13;
//static const uint8_t D6   = 12;
//static const uint8_t D5   = 14;

//GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
//GxIO_Class io(SPI, SS, D3, D4);
// GxGDEP015OC1(GxIO& io, uint8_t rst = D4, uint8_t busy = D2);
//GxEPD_Class display(io);
// or my IoT connection, busy on MISO
//GxEPD_Class display(io, D4, D6);

#define VARIOSCREEN_CS_PIN SS
#define VARIOSCREEN_DC_PIN D3
#define VARIOSCREEN_RST_PIN D4


#elif defined(ESP32)

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

#elif defined(ARDUINO_ARCH_SAMD)

// variant.h of MKR1000
//#define PIN_SPI_MISO  (10u)
//#define PIN_SPI_MOSI  (8u)
//#define PIN_SPI_SCK   (9u)
//#define PIN_SPI_SS    (24u) // should be 4?
// variant.h of MKRZERO
//#define PIN_SPI_MISO  (10u)
//#define PIN_SPI_MOSI  (8u)
//#define PIN_SPI_SCK   (9u)
//#define PIN_SPI_SS    (4u)

//GxIO_Class io(SPI, 4, 7, 6);
//GxEPD_Class display(io, 6, 5);

#define VARIOSCREEN_CS_PIN 4   
#define VARIOSCREEN_DC_PIN 7
#define VARIOSCREEN_RST_PIN A3
#define VARIOSCREEN_BUSY_PIN 3
//#define VARIOSCREEN_BUSY_PIN 5

#elif defined(_BOARD_GENERIC_STM32F103C_H_)

// STM32 Boards (STM32duino.com)
// Generic STM32F103C series
// aka BluePill
// board.h
//#define BOARD_SPI1_NSS_PIN        PA4
//#define BOARD_SPI1_MOSI_PIN       PA7
//#define BOARD_SPI1_MISO_PIN       PA6
//#define BOARD_SPI1_SCK_PIN        PA5
//enum {
//    PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13,PA14,PA15,
//  PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13,PB14,PB15,
//  PC13, PC14,PC15
//};
// variant.h
//static const uint8_t SS   = BOARD_SPI1_NSS_PIN;
//static const uint8_t SS1  = BOARD_SPI2_NSS_PIN;
//static const uint8_t MOSI = BOARD_SPI1_MOSI_PIN;
//static const uint8_t MISO = BOARD_SPI1_MISO_PIN;
//static const uint8_t SCK  = BOARD_SPI1_SCK_PIN;

//GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst = -1, int8_t bl = -1);
//GxIO_Class io(SPI, SS, 8, 9);
//  GxGDEP015OC1(GxIO& io, uint8_t rst = 9, uint8_t busy = 7);
//GxEPD_Class display(io, 9, 3);
#define VARIOSCREEN_CS_PIN SS
#define VARIOSCREEN_DC_PIN 8
#define VARIOSCREEN_RST_PIN 9
#define VARIOSCREEN_BUSY_PIN 3

#else

// pins_arduino.h, e.g. AVR
//#define PIN_SPI_SS    (10)
//#define PIN_SPI_MOSI  (11)
//#define PIN_SPI_MISO  (12)
//#define PIN_SPI_SCK   (13)

//GxIO_Class io(SPI, SS, 8, 9);
//GxIO_DESTM32L io;
//GxIO_GreenSTM32F103V io;
//GxEPD_Class display(io);

#define VARIOSCREEN_CS_PIN SS
#define VARIOSCREEN_DC_PIN 8
#define VARIOSCREEN_RST_PIN 9

#endif

#if defined(ESP8266)

#elif defined(ESP32)
#define SDCARD_CS_PIN SDCARD_SS_PIN
#elif defined(ARDUINO_ARCH_SAMD)
#define SDCARD_CS_PIN SDCARD_SS_PIN
#elif defined(_BOARD_GENERIC_STM32F103C_H_)

#elif defined(ARDUINO_AVR_PRO)
#define SDCARD_CS_PIN 14
#else

#endif

/***************************/
/*        LED              */
/***************************/

#if defined(ESP8266)
#define LED LED_BUILTIN
#elif defined(ESP32)
#define LED 2
#elif defined(ARDUINO_ARCH_SAMD)
#define LED LED_BUILTIN
#elif defined(_BOARD_GENERIC_STM32F103C_H_)
#define LED LED_BUILTIN
#elif defined(ARDUINO_AVR_PRO)
#define LED 13
#else

#endif

/****************************/
/*           Buttons        */
/****************************/

/*int pinSDA = 11;
int pinSCL = 12;*/
//#define VARIODRDY_INT_PIN 3
//#define VARIOAUDIO_PWM1_PIN A3
//#define VARIOAUDIO_PWM2_PIN A4
#define VARIOBTN_LEFT_PIN A2
#define VARIOBTN_CENTER_PIN A1
#define VARIOBTN_RIGHT_PIN D5

/*interrupts in the Zero variant:

EXTERNAL_INT_2: A0, A5, 10
EXTERNAL_INT_4: A3, 6
EXTERNAL_INT_5: A4, 7
EXTERNAL_INT_6: 8, SDA
EXTERNAL_INT_7: 9, SCL
EXTERNAL_INT_9: A2, 3
EXTERNAL_INT_10: TX, MOSI
EXTERNAL_INT_11: RX, SCK*/

#define VARIOPOWER_INT_PIN 6
//const byte interruptPin = A5;

//#define VARIO_DETECT_USB A6
#define VARIO_PIN_ALIM   D5
#define VARIO_PIN_RST    A4
// A6    Detection de connection USB
// A5    Commande de l'alimentation des cartes

/***********************/
/* The voltage divisor */
/***********************/

#if defined(ESP8266)

#elif defined(ESP32)

#elif defined(ARDUINO_ARCH_SAMD)
#define VOLTAGE_DIVISOR_PIN ADC_BATTERY
#elif defined(_BOARD_GENERIC_STM32F103C_H_)

#elif defined(ARDUINO_AVR_PRO)
#define VOLTAGE_DIVISOR_PIN 16
#else

#endif

#define VOLTAGE_DIVISOR_VALUE 1.27
#define VOLTAGE_DIVISOR_REF_VOLTAGE 3.3


/*****************************/
/*  EEPROM SOUND             */
/*****************************/

/* eeprom sound setting adresses */
#define SOUND_EEPROM_TAG 9806
#define SOUND_EEPROM_ADDR 0x30


#endif