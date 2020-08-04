#include "driver/i2s.h"
#include "freertos/queue.h"
#include <stdio.h>
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include <math.h>


#define SAMPLE_RATE     (36000)
#define I2S_NUM         (0)
#define WAVE_FREQ_HZ    (100)
#define PI              (3.14159265)
#define I2S_BCK_IO      (GPIO_NUM_13)
#define I2S_WS_IO       (GPIO_NUM_15)
#define I2S_DO_IO       (GPIO_NUM_21)
#define I2S_DI_IO       (-1)

#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)

static void setup_triangle_sine_waves(int bits)
{
    int *samples_data = malloc(((bits+8)/16)*SAMPLE_PER_CYCLE*4);
    unsigned int i, sample_val;
    double sin_float, triangle_float, triangle_step = (double) pow(2, bits) / SAMPLE_PER_CYCLE;
    size_t i2s_bytes_write = 0;

    Serial.printf("\r\nTest bits=%d free mem=%d, written data=%d\n", bits, esp_get_free_heap_size(), ((bits+8)/16)*SAMPLE_PER_CYCLE*4);

    triangle_float = -(pow(2, bits)/2 - 1);

    for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
        sin_float = sin(i * 2 * PI / SAMPLE_PER_CYCLE);
        if(sin_float >= 0)
            triangle_float += triangle_step;
        else
            triangle_float -= triangle_step;

        sin_float *= (pow(2, bits)/2 - 1);

        if (bits == 16) {
            sample_val = 0;
            sample_val += (short)triangle_float;
            sample_val = sample_val << 16;
            sample_val += (short) sin_float;
            samples_data[i] = sample_val;
        } else if (bits == 24) { //1-bytes unused
            samples_data[i*2] = ((int) triangle_float) << 8;
            samples_data[i*2 + 1] = ((int) sin_float) << 8;
        } else {
            samples_data[i*2] = ((int) triangle_float);
            samples_data[i*2 + 1] = ((int) sin_float);
        }

    }

    i2s_set_clk(I2S_NUM, SAMPLE_RATE, bits, 2);
    //Using push
    // for(i = 0; i < SAMPLE_PER_CYCLE; i++) {
    //     if (bits == 16)
    //         i2s_push_sample(0, &samples_data[i], 100);
    //     else
    //         i2s_push_sample(0, &samples_data[i*2], 100);
    // }
    // or write
    i2s_write(I2S_NUM, samples_data, ((bits+8)/16)*SAMPLE_PER_CYCLE*4, &i2s_bytes_write, 100);

    free(samples_data);
}


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
