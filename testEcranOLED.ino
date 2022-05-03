#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64);

#include "Seeed_BME280.h"
#include <Wire.h>
BME280 bme280;

void setup() {

  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1.8);
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(0,0);
  display.println("Bienvenue au FABLab\n'OpenFactory':");
  display.println("connected!");
  display.println("IP: 10.0.1.23");
  display.println("Sending val #42");
  display.setCursor(0,0);
  display.display(); // actually display all of the above
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  !bme280.init();
}

void loop() {
  float pressure;

  //get and print temperatures
  display.setCursor(0,0);
  display.print("Temp: ");
  display.print(bme280.getTemperature());
  display.println("C");

  //get and print atmospheric pressure data
  display.setCursor(0,12);
  display.print("Pressure: ");
  pressure = bme280.getPressure();
  display.print(pressure);
  display.println("Pa");

  //get and print altitude data
  display.setCursor(0,24);
  display.print("Altitude: ");
  display.print(bme280.calcAltitude(pressure));
  display.println("m");

  //get and print humidity data
  display.setCursor(0,36);
  display.print("Humidity: ");
  display.print(bme280.getHumidity());
  display.println("%");
  display.display();
  delay(10000);
  // Clear the buffer.
  display.clearDisplay();
}
