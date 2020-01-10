#include "PM2008.h"

void PM2008::begin(void)
{
    interface = &Wire;
    PM2008_I2C::begin();
}

void PM2008::begin(void *serial)
{
    if (serial == &Wire)
    {
        PM2008_I2C::begin();
        interface = &Wire;
    }
    else
    {
        pm2008_uart::begin(serial);
        interface = serial;
        sw_version = pm2008_uart::sw_version;
        serial_num = pm2008_uart::serial_num;
    }
}

void PM2008::update(void)
{
    if (interface == &Wire)
    {
        PM2008_I2C::read();
        pm1p0_grimm = PM2008_I2C::pm1p0_grimm;
        pm2p5_grimm = PM2008_I2C::pm2p5_grimm;
        pm10_grimm = PM2008_I2C::pm10_grimm;
        pm1p0_tsi = PM2008_I2C::pm1p0_tsi;
        pm2p5_tsi = PM2008_I2C::pm2p5_tsi;
        pm10_tsi = PM2008_I2C::pm10_tsi;
        number_of_0p3_um = PM2008_I2C::number_of_0p3_um;
        number_of_0p5_um = PM2008_I2C::number_of_0p5_um;
        number_of_1_um = PM2008_I2C::number_of_1_um;
        number_of_2p5_um = PM2008_I2C::number_of_2p5_um;
        number_of_5_um = PM2008_I2C::number_of_5_um;
        number_of_10_um = PM2008_I2C::number_of_10_um;        
    }
    else
    {
        pm2008_uart::update();
        pm1p0_grimm = pm2008_uart::pm1p0_grimm;
        pm2p5_grimm = pm2008_uart::pm2p5_grimm;
        pm10_grimm = pm2008_uart::pm10_grimm;
        pm1p0_tsi = pm2008_uart::pm1p0_tsi;
        pm2p5_tsi = pm2008_uart::pm2p5_tsi;
        pm10_tsi = pm2008_uart::pm10_tsi;
        number_of_0p3_um = pm2008_uart::number_of_0p3_um;
        number_of_0p5_um = pm2008_uart::number_of_0p5_um;
        number_of_1_um = pm2008_uart::number_of_1_um;
        number_of_2p5_um = pm2008_uart::number_of_2p5_um;
        number_of_5_um = pm2008_uart::number_of_5_um;
        number_of_10_um = pm2008_uart::number_of_10_um;        
    }
}