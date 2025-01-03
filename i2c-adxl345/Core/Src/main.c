#include <stdint.h>
#include "i2c.h"
#include "adxl345.h"
#include "uart.h"

extern uint8_t g_data_rec[6];
const float FOUR_G_SCALE = 0.0078;

double x, y, z;
double xg, yg, zg;

int main() {
    uart2_rxtx_init();

    // enable I2C
    I2C1_init();
    I2C1_scan_bus();

    adxl_init();

    while(1) {
        adxl_read_values(DATA_START_ADDR);

        x = (g_data_rec[1] << 8) | g_data_rec[0];
        y = (g_data_rec[3] << 8) | g_data_rec[2];
        z = (g_data_rec[5] << 8) | g_data_rec[4];

        xg = (x * FOUR_G_SCALE);
        yg = (y * FOUR_G_SCALE);
        zg = (z * FOUR_G_SCALE);
    }
}

// int main() {
//     uart2_rxtx_init();
//     I2C1_init();

//     while(1) {
// 		I2C1_scan_bus();
// 	}
// }