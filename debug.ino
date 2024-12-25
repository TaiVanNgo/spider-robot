
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_PWMServoDriver.h>

// Create an instance of the PCA9685 driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo pulse length range (adjust as needed for your servos)
#define SERVOMIN 150 // Minimum pulse length for 0°
#define SERVOMAX 600 // Maximum pulse length for 180°

#define SERVO1_CHANNEL 0 // Servo 1 on Channel 0
#define SERVO2_CHANNEL 1 // Servo 2 on Channel 1

void setup()
{
  Serial.begin(115200);

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz for servos

  Serial.println("Starting servo control...");
  Serial.println("Enter command in the format: <servo_number> <angle>");
  Serial.println("Example: '1 90' (Servo 1 to 90°)");
}

void loop()
{
  // Check if there's input from the Serial Monitor
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    input.trim();                                // Remove any extra whitespace

    // Parse the input
    int servoNumber = input.charAt(0) - '0'; // Extract servo number (1 or 2)
    int angle = input.substring(2).toInt();  // Extract angle as an integer

    // Validate input
    if ((servoNumber == 1 || servoNumber == 2) && (angle >= 0 && angle <= 180))
    {
      int pulseLength = map(angle, 0, 180, SERVOMIN, SERVOMAX);

      if (servoNumber == 1)
      {
        pwm.setPWM(SERVO1_CHANNEL, 0, pulseLength); // Set Servo 1
        Serial.print("Servo 1 moved to ");
      }
      else if (servoNumber == 2)
      {
        pwm.setPWM(SERVO2_CHANNEL, 0, pulseLength); // Set Servo 2
        Serial.print("Servo 2 moved to ");
      }
      Serial.print(angle);
      Serial.println("°");
    }
    else
    {
      Serial.println("Invalid input. Use format: <servo_number> <angle> (e.g., '1 90').");
    }
  }
}