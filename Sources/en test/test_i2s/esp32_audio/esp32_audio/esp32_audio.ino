#include "driver/i2s.h"
#include "freertos/queue.h"
#include <pgmspace.h>

#include "soundsample.h"


//i2s configuration
int i2s_num = 0; // i2s port number
i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = 16000,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
  .dma_buf_count = 8,
  .dma_buf_len = 64   //Interrupt level 1
};

i2s_pin_config_t pin_config = {
  .bck_io_num = 26,   //27, //26, //this is BCK pin
  .ws_io_num = 25,    //26, //25, // this is LRCK pin
  .data_out_num = 14, //13, //22, // this is DATA output pin
  .data_in_num = -1   //Not used
};


int i2s_write_sample_nb(uint32_t sample) {
  return i2s_write_bytes((i2s_port_t)i2s_num, (const char *)&sample, sizeof(uint32_t), 100);
}

//Main function to play samples from PROGMEM
void playPROGMEMsample(const uint8_t* audioSample) {

  uint32_t sampleSize = sizeof(audioSample) * 4;

  uint32_t counter = 0;
  //initialize i2s with configurations above
  i2s_driver_install((i2s_port_t)i2s_num, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)i2s_num, &pin_config);
  //set sample rates of i2s to sample rate of wav file
  i2s_set_sample_rates((i2s_port_t)i2s_num, 16000);


  uint32_t readData;

  while (audioSample)  {
    readData = pgm_read_dword(&audioSample[counter++]);
    if (readData == NULL) break;
    i2s_write_sample_nb(readData);
  }

  i2s_driver_uninstall((i2s_port_t)i2s_num); //stop & destroy i2s driver
}

void setup() {

  Serial.begin(115200);
  Serial.println("Serial connection OK");
  playPROGMEMsample(sample1);
}

void loop() {


}
