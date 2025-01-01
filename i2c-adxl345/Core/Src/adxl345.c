#include <stdint.h>

#include "adxl345.h"
#include "i2c.h"

char g_data;
uint8_t g_data_rec[6];

void adxl_read_address(uint8_t reg) {
    I2C1_byteRead(DEVICE_ADDR, reg, &g_data);
}

void adxl_write(uint8_t reg, char value) {
    char data[1] = {value};
    I2C1_burstWrite(DEVICE_ADDR, reg, 1, data);
}

void adxl_read_values(uint8_t reg) {
    I2C1_burstRead(DEVICE_ADDR, reg, DEVICE_DATA_READ_LEN, (char*) g_data_rec);
}

void adxl_init(void) {
    // DEVID register holds a fixed device ID code of 0xE5
    adxl_read_address(DEVID_R);
    // if(g_data != DEVID_FIXED) {}

    // set data format range
    adxl_write(DATA_FORMAT_R, DATA_FORMAT_FOUR_G);

    // reset all bits of POWER_CTL_R
    adxl_write(POWER_CTL_R, RESET_R_VAL); // test if this is necessary?

    // configure power control measure bit to measurement mode
    adxl_write(POWER_CTL_R, POWER_CTL_SET_MEASURE_B);
}