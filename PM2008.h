#ifndef PM2008_H
#define PM2008_H

#include <Arduino.h>

#define BUF_LEN   64

class PM2008
{
  public:
    void        begin(HardwareSerial *serial);
    boolean     open();
    void        update();
    boolean     close();
    
    uint8_t     status;
    char        version[16];
    String      serial_num;
    uint32_t    measuring_mode;
    uint32_t    calibration_coefficient;
    uint32_t    pm1p0_grimm;
    uint32_t    pm2p5_grimm;
    uint32_t    pm10_grimm;
    uint32_t    pm1p0_tsi;
    uint32_t    pm2p5_tsi;
    uint32_t    pm10_tsi;
    uint32_t    number_of_0p3_um;
    uint32_t    number_of_0p5_um;
    uint32_t    number_of_1_um;
    uint32_t    number_of_2p5_um;
    uint32_t    number_of_5_um;
    uint32_t    number_of_10_um;
  
  private:
    HardwareSerial  *ptrSerial;
    void        getVersion();
    void        getSerialNumber();
    byte        rxd_uart[BUF_LEN];
    byte        resp_len;
    boolean     command(byte cmd, byte data[], int data_len, int resp_len);
};

#endif
