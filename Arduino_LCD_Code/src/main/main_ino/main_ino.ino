
#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

// Library only supports hardware SPI at this time
// Connect SCLK to UNO Digital #13 (Hardware SPI clock)
// Connect MISO to UNO Digital #12 (Hardware SPI MISO)
// Connect MOSI to UNO Digital #11 (Hardware SPI MOSI)
#define RA8875_INT 3
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

void setup() 
{
  Serial.begin(9600);
  Serial.println("RA8875 start");

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_800x480)) {
    Serial.println("RA8875 Not Found!");
    while (1);
  }

  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);

  /* Switch to text mode */  
  tft.textMode();
  
  /* Render some text! */
  char string[9] = "Mr. Pour";

  /* Change the cursor location and color ... */  
  tft.textTransparent(RA8875_CYAN);
  tft.textSetCursor(280, 150);
  tft.textEnlarge(4);
  tft.textWrite(string);
  
  delay(250);
  
  tft.graphicsMode();
//  tft.drawLine(270,240,540,240,RA8875_WHITE);
  tft.fillRect(230,240,350,10,RA8875_CYAN);
  
  tft.textMode();
  char beginText[27] = "Press anywhere to begin...";
  
  tft.textEnlarge(1);
  tft.textTransparent(RA8875_WHITE);
  tft.textSetCursor(200,420);
  tft.textWrite(beginText);
  
  /* Touch mode enabled */
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true);
}

void loop() 
{
}
