#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// GPIO 21 → SDA
// GPIO 22 → SCL

// Create an instance of the PCA9685 driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo pulse length range
#define SERVOMIN 150 // Minimum pulse length for 0°
#define SERVOMAX 600 // Maximum pulse length for 180°

// Define channels for legs
#define FRONT_LEFT_HIP_CHANNEL 0  // Inner servo for the front-left leg
#define FRONT_LEFT_KNEE_CHANNEL 1 // Outer servo for the front-left leg

#define FRONT_RIGHT_HIP_CHANNEL 2  // Inner servo for the front-right leg
#define FRONT_RIGHT_KNEE_CHANNEL 3 // Outer servo for the front-right leg

#define BACK_LEFT_HIP_CHANNEL 4  // Inner servo for the back-left leg
#define BACK_LEFT_KNEE_CHANNEL 5 // Outer servo for the back-left leg

#define BACK_RIGHT_HIP_CHANNEL 6  // Inner servo for the back-right leg
#define BACK_RIGHT_KNEE_CHANNEL 7 // Outer servo for the back-right leg

void setup()
{
  Serial.begin(115200);
  Serial.println("Spider Robot Initialized. Type 'happy' to perform the happy action.");

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz for servos

  idle();
}

void loop()
{
  // Check for user input from Serial Monitor
  if (Serial.available())
  {
    String command = Serial.readStringUntil('\n'); // Read user input until newline
    command.trim();                                // Remove any extra spaces or newline characters

    // Convert command to integer
    int commandInt = command.toInt();
    if (commandInt == 1)
    {
      Serial.println("Performing Happy Action...");
      happyAction();
      idle(); // Return to idle position after the action
    }
    else if (commandInt == 2)
    {
      Serial.println("Performing Say Hi Action...");
      sayHi();
      idle(); // Return to idle position after the action
    }
    else if (commandInt == 3)
    {
      Serial.println("Performing Pull up Action...");
      pullUpAction();
      idle(); // Return to idle position after the action
    }
    else
    {
      Serial.println("Unknown command. Returning to idle...");
      idle();
    }
  }
  else
  {
    idle(); // Keep the robot in the default idle state if no input
  }

  idle();
}

int calculatePulseLength(int angle)
{
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void idle()
{
  // Default positions for knees (standing position at 0°)
  // int kneeDefault = map(0, 0, 180, SERVOMIN, SERVOMAX);
  int kneeDefault = calculatePulseLength(0);
  pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneeDefault);

  // Default positions for hips (at 45°)
  // int hipDefault = map(45, 0, 180, SERVOMIN, SERVOMAX);
  int hipDefault = calculatePulseLength(45);

  pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, hipDefault);
  pwm.setPWM(FRONT_RIGHT_HIP_CHANNEL, 0, hipDefault);
  pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, hipDefault);
  pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, hipDefault);
}

void moveForward()
{
  // Define pulse lengths for movements
  int liftKnee = map(45, 0, 180, SERVOMIN, SERVOMAX); // Knee lifted position
  int lowerKnee = map(0, 0, 180, SERVOMIN, SERVOMAX); // Knee default position
}

void pullUpAction()
{
  // int frontKnee = map(0, 0, 180, SERVOMIN, SERVOMAX);
  // int backKnee = map(20, 0, 180, SERVOMIN, SERVOMAX);

  // int backHipLeft = map(90, 0, 180, SERVOMIN, SERVOMAX);
  // int backHipRight = map(0, 0, 180, SERVOMIN, SERVOMAX);

  int frontKnee = calculatePulseLength(0);
  int backKnee = calculatePulseLength(20);

  int backHipLeft = calculatePulseLength(90);
  int backHipRight = calculatePulseLength(0);

  pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, frontKnee);
  pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, frontKnee);
  pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, backKnee);
  pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, backKnee);

  pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, backHipLeft);
  pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, backHipRight);

  int pullUpCount = 0;

  // pull up 3 reps
  while (pullUpCount < 3)
  {
    for (int angle = 0; angle <= 45; angle++)
    {
      pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
      pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, calculatePulseLength(angle));

      delay(20);
    }

    for (int angle = 45; angle >= 0; angle--)
    {
      pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
      pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, calculatePulseLength(angle));

      delay(20);
    }

    pullUpCount++;
  }
}

void happyAction()
{
  int kneeChannels[] = {FRONT_LEFT_KNEE_CHANNEL, FRONT_RIGHT_KNEE_CHANNEL, BACK_LEFT_KNEE_CHANNEL, BACK_RIGHT_KNEE_CHANNEL};
  int count = 0;

  while (count <= 4)
  {
    for (int angle = 0; angle <= 45; angle++)
    {
      int pulseLength = calculatePulseLength(angle);
      for (int i = 0; i < 4; i++)
      {
        pwm.setPWM(kneeChannels[i], 0, pulseLength);
      }
      delay(20);
    }

    for (int angle = 45; angle >= 0; angle--)
    {
      int pulseLength = calculatePulseLength(angle);
      for (int i = 0; i < 4; i++)
      {
        pwm.setPWM(kneeChannels[i], 0, pulseLength);
      }
      delay(20);
    }

    count++;
  }
}