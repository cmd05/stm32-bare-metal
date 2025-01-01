#pragma once

// adxl345 has 8 bit registers
// register addresses
#define DEVID_R                     (0x00)
#define DEVICE_ADDR                 (0x53) // ok, tested with arduino i2c scan
#define DATA_FORMAT_R               (0x31)
#define POWER_CTL_R                 (0x2D)
#define DATA_START_ADDR             (0x32) // DATAX0

#define DATA_FORMAT_FOUR_G          (0x01) // set range to +-4g
#define RESET_R_VAL                 (0x00) // reset value of a register to zero
#define POWER_CTL_SET_MEASURE_B     (0x08) // setting of 1 places the part into measurement mode

#define DEVID_FIXED                 (0xE5)
#define DEVICE_DATA_READ_LEN        (6)   // X0, X1, Y0, Y1, Z0, Z1

void adxl_init(void);
void adxl_read_values(uint8_t reg);