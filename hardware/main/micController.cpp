#include <driver/i2s.h>

#define I2S_PORT I2S_NUM_0

#define PIN_BCK  18
#define PIN_WS   17
#define PIN_DATA 10

#define SAMPLE_RATE 16000

void setupMic() {
    i2s_config_t config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 512,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_pin_config_t pins = {
        .bck_io_num = PIN_BCK,
        .ws_io_num = PIN_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = PIN_DATA
    };

    i2s_driver_install(I2S_PORT, &config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pins);
    i2s_zero_dma_buffer(I2S_PORT);
}

bool readMic(void* buffer, size_t maxBytes, size_t* bytesRead) {
    if (buffer == nullptr || bytesRead == nullptr) {
        return false;
    }

    esp_err_t result = i2s_read(
        I2S_PORT,
        buffer,
        maxBytes,
        bytesRead,
        portMAX_DELAY
    );

    return (result == ESP_OK && *bytesRead > 0);
}