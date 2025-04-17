#include <Arduino.h>

#define led1 2
#define led2 4
#define button1 7
#define button2 8

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// void testdrawchar(void) {
//   display.clearDisplay();

//   display.setTextSize(1);      // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE); // Draw white text
//   display.setCursor(0, 0);     // Start at top-left corner
//   display.cp437(true);         // Use full 256 char 'Code Page 437' font

//   // Not all the characters will fit on the display. This is normal.
//   // Library will draw what it can and the rest will be clipped.
//   for(int16_t i=0; i<256; i++) {
//     if(i == '\n') display.write(' ');
//     else          display.write(i);
//   }

//   display.display();
//   delay(2000);
// }

int bpm = 100;
int beats = 4;
unsigned long myTime;

void testdrawstyles(void)
{
  display.clearDisplay();

  display.setTextSize(1);              // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x"));
  display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}

// void testscrolltext(void) {
//   display.clearDisplay();

//   display.setTextSize(2); // Draw 2X-scale text
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(10, 0);
//   display.println(F("scroll"));
//   display.display();      // Show initial text
//   delay(100);

//   // Scroll in various directions, pausing in-between:
//   display.startscrollright(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrollleft(0x00, 0x0F);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
//   display.startscrolldiagright(0x00, 0x07);
//   delay(2000);
//   display.startscrolldiagleft(0x00, 0x07);
//   delay(2000);
//   display.stopscroll();
//   delay(1000);
// }

void displaybpm()
{
  display.clearDisplay();
  display.setTextSize(3.5);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.println("BPM is " + String(bpm));
  display.display();
}


// Variables will change:
int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated


void setup()
{
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  // pinMode(5, OUTPUT);
  // pinMode(6, OUTPUT);
  // pinMode(7, OUTPUT);
  // pinMode(8, OUTPUT);
  // pinMode(9, OUTPUT);
  // digitalWrite(5, HIGH);
  // digitalWrite(6, HIGH);
  // digitalWrite(7, HIGH);
  // digitalWrite(8, HIGH);
  // digitalWrite(9, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);

  Wire.begin();


  Serial.begin(115200);
  Serial.println(137);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(1);
    Serial.println("Hi");
    Serial.println('Hi');
    Serial.println(F("SSD1306 allocation failed"));
    while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      delay(30);
      Serial.println("Not yet connected"); 
    }
    
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  displaybpm();

  // Draw a single pixel in white
  // display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  // display.display();
  // delay(2000);

  // testdrawchar();      // Draw characters of the default font

  // testdrawstyles(); // Draw 'stylized' characters

  // testscrolltext();    // Draw scrolling text

  // Invert and restore display, pausing in-between
}

int counter = 0;
unsigned long lastPress = 0;

void loop() {
  delay(20);
  unsigned long currentMillis = millis();
  if (digitalRead(button1) == LOW && currentMillis - lastPress >= 100) {
    bpm--;
    lastPress = millis();
  }
  if (digitalRead(button2) == LOW && currentMillis - lastPress >= 100) {
    bpm++;
    lastPress = millis();
  }
  displaybpm();
  if (currentMillis - previousMillis >= (60*500/bpm)-10) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    if (counter < 2) {
      digitalWrite(led1, ledState);
    } else {
      digitalWrite(led2, ledState);
    }
    counter++;
    if (counter == 8) {
      counter = 0;
    }
    Serial.println(counter);
  }
}