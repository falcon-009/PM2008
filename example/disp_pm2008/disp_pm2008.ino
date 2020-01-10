#include <LiquidCrystal.h>
#include <PM2008.h>

PM2008 dust_sensor;


//Pin assignments for LCD Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

void setup() 
{ 
    // Setup Serial Monitor uart
    Serial.begin(115200);

    // Setup LCD
    lcd.begin(16, 2);

    // Initialize dust dust_sensor
    dust_sensor.begin(&Serial3);
    // Display Sensor sw version.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor SW Ver.");
    lcd.setCursor(0, 1);
  //  lcd.print(dust_sensor.version);
    for (int i = 0; i < 10; i++) 
      lcd.print(dust_sensor.version[i]);
    delay(2000);

    // Display Sensor sw version.
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Serial No.");
    lcd.setCursor(0, 1);
    lcd.print(dust_sensor.serial_num);
    delay(2000);

    // Set the PWM output pin mode
    dust_sensor.open();

    delay(1000);
}

void loop() 
{ 
    static long t, prev_t;

    t = millis();
    if (t - prev_t > 1000)
    {
        dust_sensor.update();
        ui();
        prev_t = t;
    }
}

void displayPm(void)
{
    static uint16_t val = 0;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PM1.0 PM2.5 PM10");
    lcd.setCursor(1,1);
    lcd.print(dust_sensor.pm1p0_tsi);
    lcd.setCursor(7,1);
    lcd.print(dust_sensor.pm2p5_tsi);
    lcd.setCursor(13,1);
    lcd.print(dust_sensor.pm10_tsi);
}

void ui(void)
{
    displayPm();
}
