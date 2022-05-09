#include <SPI.h>
#include <FastLED.h>
#include "Seeed_BME280.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//====================
//Parametre de l'ecran
//====================
//Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64);

//=======================
// Parametrer anneau led
//=======================

// How many leds in your strip?
#define NUM_LEDS 12
#define DATA_PIN 3
#define BRIGHTNESS  32
CRGB leds[NUM_LEDS];  // Define the array of leds

// ===========================
//Parametrer capteur BME-280
BME280 bme280;
// ===========================

// =======================
// Paramètrages écran OLED
// =======================
#define nombreDePixelsEnLargeur   128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur   64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED           -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED       0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
Adafruit_SSD1306 display(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// ================
// Image à afficher
// ================
#define largeurDeLimage           64          // Largeur de l'image à afficher, en pixels
#define hauteurDeLimage           64          // Hauteur de l'image à afficher, en pixels

// 'logoOpenfactoryZoomacomArduino', 64x64px
const unsigned char epd_bitmap_logoOpenfactoryZoomacomArduino [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xfc, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00,
  0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
  0x00, 0x1f, 0xff, 0xff, 0xfe, 0x0f, 0xf0, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x07, 0xf8, 0x00,
  0x00, 0x3f, 0xfe, 0x0f, 0x00, 0x1f, 0xfc, 0x00, 0x00, 0x7f, 0xf0, 0x07, 0x01, 0xff, 0xfe, 0x00,
  0x00, 0x7f, 0xc0, 0x03, 0x0f, 0xff, 0xfe, 0x00, 0x00, 0x7f, 0x00, 0x21, 0x0f, 0xff, 0xfe, 0x00,
  0x00, 0xfe, 0x03, 0xf0, 0x1f, 0xff, 0xff, 0x00, 0x00, 0xfe, 0x07, 0xf8, 0x1f, 0xff, 0xff, 0x00,
  0x00, 0xfc, 0x0f, 0xf8, 0x00, 0x7f, 0xff, 0x00, 0x00, 0xfc, 0x1f, 0xf8, 0x00, 0x7f, 0xff, 0x00,
  0x00, 0xf8, 0x3f, 0xf0, 0x03, 0xff, 0xfe, 0x00, 0x00, 0xf8, 0x7f, 0xf0, 0x1f, 0xff, 0xfe, 0x00,
  0x01, 0xf8, 0x7f, 0xe0, 0x3f, 0xff, 0xfc, 0x00, 0x01, 0xfc, 0x7f, 0xc0, 0x7f, 0xff, 0xfc, 0x00,
  0x01, 0xfc, 0x3f, 0x80, 0x7f, 0xff, 0xf8, 0x00, 0x01, 0xfe, 0x3f, 0x00, 0xff, 0xff, 0xf0, 0x00,
  0x01, 0xff, 0x1c, 0x10, 0xff, 0xff, 0xf0, 0x00, 0x01, 0xff, 0x80, 0x31, 0xff, 0xff, 0xe0, 0x00,
  0x01, 0xff, 0xc0, 0xf1, 0xff, 0xff, 0x80, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x0f, 0xff, 0xfe, 0x00, 0x0f, 0xfb, 0x80, 0x00, 0x03, 0xff, 0xe0, 0x00, 0x0e, 0xde, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x0e, 0xdf, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
  0x01, 0xf7, 0xbe, 0xfe, 0x7b, 0xcf, 0x7f, 0x80, 0x00, 0x6c, 0xb6, 0xfb, 0xdb, 0x19, 0x6d, 0x80,
  0x00, 0x6c, 0xb7, 0x93, 0xda, 0x19, 0x6d, 0x80, 0x00, 0xcc, 0xb7, 0x93, 0xdb, 0x19, 0x6d, 0x80,
  0x01, 0xe7, 0xbf, 0x83, 0xfb, 0xdf, 0x61, 0x80, 0x01, 0xf7, 0x1c, 0x82, 0x79, 0xce, 0x41, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 528)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
  epd_bitmap_logoOpenfactoryZoomacomArduino
};


void setup() {
  //================================================================================
  // Choix des LED a adresser (en fonction des led sur le ruban)
  //================================================================================
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 40); // réglage de la puissance des led


  //======================================
  // Reglage de l'ecran OLED au démarrage
  //======================================

  Serial.println("OLED FeatherWing test");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(5000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  // text display tests
  display.setTextSize(1.8);
  display.setTextColor(SSD1306_INVERSE);
  display.setCursor(0, 0);
  display.println("Bienvenue au FABLab\n'OpenFactory':");
  display.println("connected!");
  display.println("IP: 10.0.1.23");
  display.println("Sending val #42");
  display.setCursor(0, 0);
  display.display(); // actually display all of the above
  delay(3000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  !bme280.init();
}

void loop() {

  //==========================================
  //Relever les mesures du capteur BME-280
  //==========================================
  float pressure;
  float pression;

  //get and print temperatures
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(bme280.getTemperature());
  display.println("C");

  //get and print atmospheric pressure data
  display.setCursor(0, 12);
  display.print("Pressure: ");
  pressure = bme280.getPressure();
  display.print(pressure);
  display.println("Pa");

  //get and print altitude data
  display.setCursor(0, 24);
  display.print("Altitude: ");
  display.print(bme280.calcAltitude(pressure));
  display.println("m");

  //get and print humidity data
  display.setCursor(0, 36);
  display.print("Humidity: ");
  display.print(bme280.getHumidity());
  //Humidity = bme280.getHumidity();
  display.println("%");
  display.display();

  //get and print atmospheric pressure data
  display.setCursor(0, 48);
  display.print("PNM: ");
  pression = (pressure + (bme280.calcAltitude(pressure)/8.3));
  display.print(pression);
  display.println("Pa");
  display.display();
  delay(2000);

  //========================================================
  //Parametrer l'anneau de LED en fonction de l'humidité
  //========================================================
  if (bme280.getHumidity() <= 40) {
    fill_solid (leds, NUM_LEDS, CRGB::FireBrick);
    FastLED.show();
  }
  else {
    fill_solid (leds, NUM_LEDS, CRGB::OrangeRed);
    FastLED.show();
  }
  // Clear the buffer.
  display.clearDisplay();
}
