#include <stdint.h>

#include "adxl345.h"
#include "spi.h"

// multibyte mode: To read or write multiple bytes in a single transmission, the 
// multiple-byte bit, located after the R/W bit in the first byte 
// transfer (MB), must be set. After the register addressing and the
// first byte of data, each subsequent set of clock pulses (eight clock pulses)
// causes the ADXL345 to point to the next register for a read or write.
// This shifting continues until the clock pulses cease and CS is deasserted

void adxl_read(uint8_t addr, uint8_t *rxdata) {
    uint8_t address = addr | ADXL_MULTIBYTE_EN | ADXL_SPI_READ;

    // enable slave
    spi1_cs_enable();

    // transmit address
    spi1_transmit(&address, 1);

    // read 6 bytes
    spi1_receive(rxdata, DEVICE_DATA_READ_LEN);

    // disable slave
    spi1_cs_disable();
}

void adxl_write(uint8_t addr, char value) {
    uint8_t data[2];

    // place address and enable multi-byte mode
    data[0] = addr | ADXL_MULTIBYTE_EN | ADXL_SPI_WRITE;

    // place data into buffer
    data[1] = value;

    // enable slave
    spi1_cs_enable();

    // transmit {<8 bit> address, <8 bit> data} to the slave
    spi1_transmit(data, 2);

    // disable slave
    spi1_cs_disable();
}

void adxl_init(void) {
    // by default SPI 4 wire mode is set in DATA_FORMAT register

    // DEVID register holds a fixed device ID code of 0xE5
    uint8_t devid;
    adxl_read(DEVID_R, &devid);
    // if(devid != DEVID_FIXED) {}

    // set data format range
    adxl_write(DATA_FORMAT_R, DATA_FORMAT_FOUR_G);

    // // reset all bits of POWER_CTL_R
    // adxl_write(POWER_CTL_R, RESET_R_VAL); // test if this is necessary?

    // configure power control measure bit to measurement mode
    adxl_write(POWER_CTL_R, POWER_CTL_SET_MEASURE_B);
}