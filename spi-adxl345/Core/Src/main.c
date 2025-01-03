#include <stdint.h>
#include "spi.h"
#include "adxl345.h"
#include "uart.h"

double x, y, z;
double xg, yg, zg;
const int FOUR_G_SCALE = 0.0078;

uint8_t g_data_rec[6];

int main() {
    uart2_rxtx_init();

    // enable SPI
    spi1_gpio_init();
    spi1_init();

    adxl_init();

    while(1) {
        adxl_read(DATA_START_ADDR, g_data_rec);

        x = (g_data_rec[1] << 8) | g_data_rec[0];
        y = (g_data_rec[3] << 8) | g_data_rec[2];
        z = (g_data_rec[5] << 8) | g_data_rec[4];

        xg = (x * FOUR_G_SCALE);
        yg = (y * FOUR_G_SCALE);
        zg = (z * FOUR_G_SCALE);
    }
}