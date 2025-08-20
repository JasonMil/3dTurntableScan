/* Works with lilygo T-Display but select ESP32 Dev Module as the board */
/* board manager esp32 by Espressif Systems use version 3.3.0 dont update */
/* Library TFT_eSPI by bodmer version 2.5.43 dont update */

#define STEPPER_PIN_1 17
#define STEPPER_PIN_2 2
#define STEPPER_PIN_3 15
#define STEPPER_PIN_4 13

#include <TFT_eSPI.h>       // Hardware-specific library
#include <SPI.h>
#include <BleKeyboard.h>
#include <AccelStepper.h>

// 28BYJ-48 has 64 steps per rev * 64 gear reduction = 4096 steps per revolution
const int stepsPerRevolution = 4096;

// 3.6 degrees corresponds to: (3.6 / 360) * 4096 ≈ 41 steps
const int stepSize = 41;

// Photo counter
int number = 0;

// Start button
const int buttonPin = 21;
bool lastButtonState = HIGH;

TFT_eSPI tft = TFT_eSPI();
BleKeyboard bleKeyboard("ESP BLE", "lilly", 100);

// Create AccelStepper object (FULL4WIRE, pin1, pin3, pin2, pin4 order is important for 28BYJ-48)
AccelStepper stepper(AccelStepper::FULL4WIRE, STEPPER_PIN_1, STEPPER_PIN_3, STEPPER_PIN_2, STEPPER_PIN_4);

void setup(void) {
  pinMode(buttonPin, INPUT_PULLUP);

  bleKeyboard.begin();
  Serial.begin(115200);

  tft.init();
  tft.fillScreen(TFT_BLACK);

  // Set max speed and acceleration
  stepper.setMaxSpeed(1000);    // steps per second
  stepper.setAcceleration(200); // smoother motion

  delay(5000);

  if (bleKeyboard.isConnected()) {
    tft.drawString("Bluetooth OK", 10, 10);
    tft.drawString("Press button to start", 10, 24);
    delay(5000);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Bluetooth Fail", 10, 10);
    tft.drawString("Check Phone is bluetooth enabled", 10, 24);
  }
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
      Serial.println("Starting loop");
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Taking Pictures", 10, 24);

      for (int i = 0; i < 360 / 3.6; i++) {
        // Move 41 steps (3.6 degrees)
        long targetPos = stepper.currentPosition() + stepSize;
        stepper.moveTo(targetPos);

        // Run until the stepper reaches the target
        while (stepper.distanceToGo() != 0) {
          stepper.run();
        }

        delay(1000);  // pause for motor to settle

        tft.drawString(String(number), 30, 100, 7);
        Serial.println(String(number));
        Serial.println("Taking Photo");

        // Uncomment when ready to trigger phone camera
        //bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
          bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
          delay(100);  // small delay so the phone registers it
          bleKeyboard.release(KEY_MEDIA_VOLUME_UP);

        number++;
        delay(1000);  // wait before next photo
      }

      number = 0;
      delay(5000);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("Press button to start", 10, 24);
    }
  }

  lastButtonState = buttonState;
}


/*Works with lilygo T-Display  but select ESP32 Dev Module as the board*/

/* board manager esp32 by Espressif Systems use version 3.3.0 dont update to a later version */
/* Library TFT_eSPI by bodmer version 2.5.43 dont update */
/*

#define STEPPER_PIN_1 17  //9
#define STEPPER_PIN_2 2  //10
#define STEPPER_PIN_3 15  //11
#define STEPPER_PIN_4 13  //12

#include <TFT_eSPI.h>       // Hardware-specific library
#include <SPI.h>
#include <BleKeyboard.h>
#include <Stepper.h>

// 28BYJ-48 has 64 steps per revolution * 64 gear reduction = 4096 total steps per 360°
const int stepsPerRevolution = 4096;

// 3.6 degrees corresponds to: (3.6 / 360) * 4096 = 41 steps
//const int stepSize = 41;
const int stepSize = 20;

//Photo taken counter
int number = 0;

//Start button
const int buttonPin = 21;


bool lastButtonState = HIGH; // Track last state (HIGH = not pressed)

TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
BleKeyboard bleKeyboard("ESP BLE", "lilly", 100);  // Device name, manufacturer, battery level

Stepper stepper(stepsPerRevolution, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4);         // IN1, IN2, IN3, IN4        

void setup(void) {
  
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  
  pinMode(buttonPin, INPUT_PULLUP);
  
  bleKeyboard.begin();
  Serial.begin(115200); 

  tft.init();
  tft.fillScreen(TFT_BLACK);

  stepper.setSpeed(10);

  delay(5000);

if (bleKeyboard.isConnected()) {

        tft.drawString("Bluetooth OK",10,10);
        tft.drawString("Press button to start",10,24);  
       delay(5000);
  
  } else {
  
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Bluetooth Fail",10,10);  
    tft.drawString("Check Phone is bluetooth enabled",10,24);  
  }

}

void loop() {

bool buttonState = digitalRead(buttonPin); // Read current button state

  
  if (buttonState != lastButtonState) {
    if (buttonState == LOW) {
    
            Serial.println("Starting loop");      
            tft.fillScreen(TFT_BLACK);  //Clear Screen
            tft.drawString("Taking Pictures",10,24); 
            
              for (int i = 0; i < 360 / 3.6; i++) {
                
                                                      stepper.step(stepSize);
                                                      // two pauses allows movement and photo to be taken
                                                      delay (1000);

                                                      tft.drawString(String(number),30,100,7);
                                                      Serial.println(String(number));
                                        
                                                      Serial.println("Taking Photo");  
                                                  //   bleKeyboard.print("KEY_MEDIA_VOLUME_UP");// take picture
                                                      //bleKeyboard.write("KEY_MEDIA_VOLUME_UP");// take picture  not sure if this is the correct way to process
                                                      
                                                      number++;
                                                      
                                                      delay(1000);  // Wait 1 seconds before sending again

                                                  }       
          number = 0;
          delay(5000);
          tft.fillScreen(TFT_BLACK);  //Clear Screen
          tft.drawString("Press button to start",10,24); 
  
          }
  
  }
}    

*/