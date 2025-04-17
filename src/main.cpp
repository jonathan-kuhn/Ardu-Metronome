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
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



#define led1 16
#define led2 14
#define button1 12
#define button2 13


int bpm = 120;
int timing = 4; //Time signature, 4/4 by default

void displaybpm()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BPM is " + String(bpm));
  display.display();
}
void displaytiming()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(String(timing)+"/4");
  display.println("Timing");
  display.display();
}


void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.println("Starting...");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Made by");
  display.println("Ducttape");
  display.println("Engineer");
  display.display();
  display.setTextSize(3.5);
  delay(1750);
}

int counter = 0;
unsigned long lastButtonPress1 = 0;
unsigned long lastButtonPress2 = 0;
int bpmincreased = 0;  // Button press counter, to make sure we don't increase BPM at the start of a short press
int bpmdecreased = 0;


int ledState = LOW;  // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;  // will store last time LED was updated


int changeTiming(int timing) {
  timing++;
  if (timing > 8) {
    timing = 1;
  }
  return timing;
}

void loop() {
  unsigned long currentMillis = millis();
  if (digitalRead(button1) == LOW && digitalRead(button2) == LOW) {
    timing = changeTiming(timing);
    displaytiming();
    counter = 0;
    ledState = LOW;
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(500);
  }
  
  // Button 1 (decrease BPM)
  if (digitalRead(button1) == LOW && currentMillis - lastButtonPress1 >= 100) {
    if (bpmdecreased == 0 || bpmdecreased > 2) { //logic to prevent the bpm from going down by multiple units when the button is pressed for a short duration
      bpm = max(1, bpm - 1);  // Prevent BPM from going to 0 or negative
    }
    lastButtonPress1 = currentMillis;
    bpmdecreased++;
  }
  
  // Button 2 (increase BPM)
  if (digitalRead(button2) == LOW && currentMillis - lastButtonPress2 >= 100) {
    if (bpmincreased == 0 || bpmincreased > 2) {  //logic to prevent the bpm from going up by multiple units when the button is pressed for a short duration
      bpm++;
    }
    lastButtonPress2 = currentMillis;
    bpmincreased++;
  }
  if (digitalRead(button1) == HIGH) { //Button 1 is not pressed - reset counter
    bpmdecreased = 0;
  }
  if (digitalRead(button2) == HIGH) { //Button 2 is not pressed - reset counter
    bpmincreased = 0;
  }
 
  displaybpm();    // Update display
  
  // Calculate the half-beat interval in milliseconds
  // (A full beat period = 60000 / bpm, so half beat = 30000 / bpm)
  unsigned long halfBeatInterval = 30000UL / bpm;
  
  // Toggle LED state based on timing for a consistent beat
  if (currentMillis - previousMillis >= halfBeatInterval) {
    previousMillis = currentMillis;
    
    // Toggle LED state
    ledState = (ledState == LOW) ? HIGH : LOW;
    
    // Alternate between two LEDs based on a counter
    if (counter < 2) {
      digitalWrite(led1, ledState);
    } else {
      digitalWrite(led2, ledState);
    }
    
    counter = (counter + 1) % (timing*2);  // Increment counter and wrap around
  }
}