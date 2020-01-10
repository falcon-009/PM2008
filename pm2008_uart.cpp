#include "pm2008_uart.h"

#define HEAD_CMD  0x11
#define HEAD_RESP 0x16

#define PM2008_UART_FRAME_HEADER                            0x11
#define PM2008_UART_CMD_OPEN_CLOSE_MEASUREMENT              0x0c
#define PM2008_UART_CMD_READ_MEASUREMENT_RESULT             0x0b
#define PM2008_UART_CMD_SETUP_READ_MEASUREMENT_TIME         0x0d
#define PM2008_UART_CMD_SETUP_READ_TIME_WORKING_MODE        0x05
#define PM2008_UART_CMD_SETUP_READ_DYNAMIC_WORKING_MODE     0x06
#define PM2008_UART_CMD_SETUP_WARM_MODE                     0x08
#define PM2008_UART_CMD_SETUP_READ_CALIBRATED_COEFFICIENT   0x07
#define PM2008_UART_CMD_READ_SW_VERSION                     0x1e
#define PM2008_UART_CMD_READ_SERIALn_NUMBER                  0x1f

byte chksum(byte *buf, int len);

// pm2008 sensor uart command packets
const byte cmd_open[] =  {HEAD_CMD, 0x03, 0x0c, 0x02, 0x1e, 0xc0};
const byte cmd_close[] = {HEAD_CMD, 0x03, 0x0c, 0x01, 0x1e, 0xc1};
const byte cmd_update[] = {HEAD_CMD, 0x02, 0x0b, 0x07, 0xdb};
const byte cmd_ver[] = {HEAD_CMD, 0x01, 0x1e, 0xd0};
const byte cmd_serial_num[] = {HEAD_CMD, 0x01, 0x1f, 0xcf};

void pm2008_uart::begin(HardwareSerial *serial)
{
    ptrSerial = serial;
    // Initialize the serial port
    ptrSerial->begin(9600);
  
    getVersion();
    getSerialNumber();
}

boolean pm2008_uart::command(byte cmd, byte data[], int data_len, int resp_len)
{
    boolean res = false;
    byte cmd_buf[16] = {0x11, (byte) (data_len+1), cmd};

    for (int i = 0; i < data_len; i++)
        cmd_buf[i+3] = data[i];

    cmd_buf[data_len+3] = chksum(cmd_buf, data_len + 2);
    
    ptrSerial->write(cmd_buf, data_len+4);
    delay(10);

    if (ptrSerial->available()) 
    {
        int len = ptrSerial->readBytes(rxd_uart, resp_len);

        if (len == resp_len && rxd_uart[0] == HEAD_RESP && chksum(rxd_uart, rxd_uart[1]+2) == rxd_uart[len-1])
        {
        res = true;
        }
    }

    return res;
}

void pm2008_uart::getVersion() 
{
  // Sensor SW Version 출력
  resp_len = 17;
  ptrSerial->write(cmd_ver, 4);
  delay(50);
  sw_version = "";

  if (ptrSerial->available()) {
    int len = ptrSerial->readBytes(rxd_uart, resp_len);

    for (int i = 3; i < len - 3; i++) 
    {
      // version[i-3] = rxd_uart[i] == ' ' ? '_' : rxd_uart[i]; // ' ' -> '_'
      // Serial.print(version[i-3]);
      sw_version += (char) rxd_uart[i];
    }
  }
  // Serial.println();
  Serial.println(sw_version);
}

void pm2008_uart::getSerialNumber(void)
{
  // Read Serial number
  resp_len = 14;
  ptrSerial->write(cmd_serial_num, 4);
  delay(50);

  if (ptrSerial->available()) {
    byte len = ptrSerial->readBytes(rxd_uart, resp_len);
    char temp[4];
    
    for(int i = 3; i < len-2; i = i + 2) {
      sprintf(temp, "%04d", (rxd_uart[i] * 256 + rxd_uart[i+1]));
      serial_num += temp;
    }
    Serial.println(serial_num);
  }
}

void pm2008_uart::update()
{
  // Write the command to read the data from uart
  const byte cmd_read_measurement[] = {0x11, 0x02, 0x0b, 0x07, 0xdb};
  ptrSerial->write(cmd_read_measurement, 5);

  if (ptrSerial->available())
  {
    byte len = ptrSerial->readBytes(rxd_uart, 56);

    if (len == 56) 
    {
      byte checksum = chksum(rxd_uart, 55);
      if (checksum != 0 && checksum == rxd_uart[55]) {
        #if 0
        pm1p0_grimm      = rxd_uart[3]*16777216 + rxd_uart[4]*65536 + rxd_uart[5]*256 + rxd_uart[6];
        pm2p5_grimm      = rxd_uart[7]*16777216 + rxd_uart[8]*65536 + rxd_uart[9]*256 + rxd_uart[10];
        pm10_grimm       = rxd_uart[11]*16777216 + rxd_uart[12]*65536 + rxd_uart[13]*256 + rxd_uart[14];
        pm1p0_tsi        = rxd_uart[15]*16777216 + rxd_uart[16]*65536 + rxd_uart[17]*256 + rxd_uart[18];
        pm2p5_tsi        = rxd_uart[19]*16777216 + rxd_uart[20]*65536 + rxd_uart[21]*256 + rxd_uart[22];
        pm10_tsi         = rxd_uart[23]*16777216 + rxd_uart[24]*65536 + rxd_uart[25]*256 + rxd_uart[26];
        number_of_0p3_um = rxd_uart[27]*16777216 + rxd_uart[28]*65536 + rxd_uart[29]*256 + rxd_uart[30];
        number_of_0p5_um = rxd_uart[31]*16777216 + rxd_uart[32]*65536 + rxd_uart[33]*256 + rxd_uart[34];
        number_of_1_um   = rxd_uart[35]*16777216 + rxd_uart[36]*65536 + rxd_uart[37]*256 + rxd_uart[38];
        number_of_2p5_um = rxd_uart[39]*16777216 + rxd_uart[40]*65536 + rxd_uart[41]*256 + rxd_uart[42];
        number_of_5_um   = rxd_uart[43]*16777216 + rxd_uart[44]*65536 + rxd_uart[45]*256 + rxd_uart[46];
        number_of_10_um  = rxd_uart[47]*16777216 + rxd_uart[48]*65536 + rxd_uart[49]*256 + rxd_uart[50];
        #else
        pm1p0_grimm      = (uint32_t)rxd_uart[3] <<24 | (uint32_t)rxd_uart[4] <<16 | (uint32_t)rxd_uart[5] <<8 | (uint32_t)rxd_uart[6];
        pm2p5_grimm      = (uint32_t)rxd_uart[7] <<24 | (uint32_t)rxd_uart[8] <<16 | (uint32_t)rxd_uart[9] <<8 | (uint32_t)rxd_uart[10];
        pm10_grimm       = (uint32_t)rxd_uart[11]<<24 | (uint32_t)rxd_uart[12]<<16 | (uint32_t)rxd_uart[13]<<8 | (uint32_t)rxd_uart[14];
        pm1p0_tsi        = (uint32_t)rxd_uart[15]<<24 | (uint32_t)rxd_uart[16]<<16 | (uint32_t)rxd_uart[17]<<8 | (uint32_t)rxd_uart[18];
        pm2p5_tsi        = (uint32_t)rxd_uart[19]<<24 | (uint32_t)rxd_uart[20]<<16 | (uint32_t)rxd_uart[21]<<8 | (uint32_t)rxd_uart[22];
        pm10_tsi         = (uint32_t)rxd_uart[23]<<24 | (uint32_t)rxd_uart[24]<<16 | (uint32_t)rxd_uart[25]<<8 | (uint32_t)rxd_uart[26];
        number_of_0p3_um = (uint32_t)rxd_uart[27]<<24 | (uint32_t)rxd_uart[28]<<16 | (uint32_t)rxd_uart[29]<<8 | (uint32_t)rxd_uart[30];
        number_of_0p5_um = (uint32_t)rxd_uart[31]<<24 | (uint32_t)rxd_uart[32]<<16 | (uint32_t)rxd_uart[33]<<8 | (uint32_t)rxd_uart[34];
        number_of_1_um   = (uint32_t)rxd_uart[35]<<24 | (uint32_t)rxd_uart[36]<<16 | (uint32_t)rxd_uart[37]<<8 | (uint32_t)rxd_uart[38];
        number_of_2p5_um = (uint32_t)rxd_uart[39]<<24 | (uint32_t)rxd_uart[40]<<16 | (uint32_t)rxd_uart[41]<<8 | (uint32_t)rxd_uart[42];
        number_of_5_um   = (uint32_t)rxd_uart[43]<<24 | (uint32_t)rxd_uart[44]<<16 | (uint32_t)rxd_uart[45]<<8 | (uint32_t)rxd_uart[46];
        number_of_10_um  = (uint32_t)rxd_uart[47]<<24 | (uint32_t)rxd_uart[48]<<16 | (uint32_t)rxd_uart[49]<<8 | (uint32_t)rxd_uart[50];
        #endif
        Serial.println("update");
      }
    }
  }
}

boolean pm2008_uart::open()
{
  
}

boolean pm2008_uart::close()
{
  
}

byte chksum(byte *buf, int len)
{
  byte sum = 0;
  for (int i = 0; i < len; i++)
  {
    sum += buf[i];
  }
  return 0-sum;
}
