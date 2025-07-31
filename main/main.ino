#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Dabble.h>
#include <SoftwareSerial.h>
/*
Module Bluetooth 3.0 SPP / BLE 4.2 Dual Mode JDY-33 setup:
  - module TX -> nano D4
  - module RX -> nano D5

PCA9685 Setup:
  - GPIO A4 → SDA
  - GPIO A5  → SCL
*/

// setup for bluetooth module
SoftwareSerial mySerial(4, 5); // RX, TX

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

const int SERVO_CHANNELS[] = {
    FRONT_LEFT_HIP_CHANNEL,
    FRONT_LEFT_KNEE_CHANNEL,
    FRONT_RIGHT_HIP_CHANNEL,
    FRONT_RIGHT_KNEE_CHANNEL,
    BACK_LEFT_HIP_CHANNEL,
    BACK_LEFT_KNEE_CHANNEL,
    BACK_RIGHT_HIP_CHANNEL,
    BACK_RIGHT_KNEE_CHANNEL};

const int KNEE_CHANNELS[] = {FRONT_LEFT_KNEE_CHANNEL, FRONT_RIGHT_KNEE_CHANNEL, BACK_LEFT_KNEE_CHANNEL, BACK_RIGHT_KNEE_CHANNEL};
const int HIP_CHANNELS[] = {FRONT_LEFT_HIP_CHANNEL, FRONT_RIGHT_HIP_CHANNEL, BACK_LEFT_HIP_CHANNEL, BACK_RIGHT_HIP_CHANNEL};

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); // JDY-3 3 Default Baud Rate
  Dabble.begin(mySerial);

  Serial.println("Spider Robot Initialized. Enter 1-6 to control spider");

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz for servos

  idle();
}

void loop()
{
  Dabble.processInput(); // Process Dabble inputs

  if (GamePad.isUpPressed())
  {
    Serial.println("Moving Forward...");
    moveForward();
  }
  if (GamePad.isLeftPressed())
  {
    Serial.println("Turning Left...");
    turnLeft();
  }
  if (GamePad.isRightPressed())
  {
    Serial.println("Turning Right...");
    turnRight();
  }
  if (GamePad.isTrianglePressed())
  {
    Serial.println("Happy Action!");
    happyAction();
  }
  if (GamePad.isCrossPressed())
  {
    Serial.println("Defense Mode!");
    defenseAction();
  }
  if (GamePad.isCirclePressed())
  {
    Serial.println("Pull Up Action!");
    pullUpAction();
  }

  // Default to idle when no input is detected
  idle();
}

int calculatePulseLength(int angle)
{
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void setMultipleServos(const int channels[], int numChannels, int deg)
{
  int pulseLength = calculatePulseLength(deg);

  for (int i = 0; i < numChannels; i++)
  {
    pwm.setPWM(channels[i], 0, pulseLength);
  }
}

void setServo(int servoNum, int deg)
{
  int pulseLength = calculatePulseLength(deg);
  pwm.setPWM(SERVO_CHANNELS[servoNum], 0, pulseLength);
}

void idle()
{
  // Default position for knees and hips is 45°
  int defaultPosition = 45;

  setMultipleServos(KNEE_CHANNELS, 4, defaultPosition);
  setMultipleServos(HIP_CHANNELS, 4, defaultPosition);
}

void turnLeft()
{
  int position = 35;

  setMultipleServos(KNEE_CHANNELS, 4, position);

  for (int angle = 45; angle <= 90; angle += 5)
  {
    // Move front-left hip and back-right hip from 45deg to 90deg
    setServo(0, angle); // front-left hip
    setServo(6, angle); // back-right hip

    delay(20);
  }

  int position2 = 60;
  setMultipleServos(KNEE_CHANNELS, 4, position2);

  for (int angle = 45; angle <= 90; angle += 5)
  {
    int angle2 = 135 - angle; // For front left and back right (90 to 45)

    setServo(2, angle); // set front-right hip from 45 to 90
    setServo(4, angle); // set back-left hip from 45 to 90

    setServo(0, angle2); // set front-left hip from 90 to 45
    setServo(6, angle2); // set back-right hip from 90 to 45

    delay(20);
  }

  int position3 = 35;
  setMultipleServos(KNEE_CHANNELS, 4, position3);

  for (int angle = 90; angle >= 45; angle -= 5)
  {
    // set front-right hip and back-left hip from 90deg to 45deg (default position)
    setServo(2, angle);
    setServo(4, angle);

    delay(20);
  }
}

void turnRight()
{
  int position = 55;

  setMultipleServos(KNEE_CHANNELS, 4, position);

  for (int angle = 45; angle >= 0; angle -= 5)
  {
    // Move front-right hip and back-left hip from 45deg to 0deg
    setServo(2, angle); // front-right hip
    setServo(4, angle); // back-left hip

    delay(20);
  }

  int position2 = 30;
  setMultipleServos(KNEE_CHANNELS, 4, position2);

  for (int angle = 45; angle >= 0; angle -= 5)
  {
    int angle2 = 45 - angle; // Calculate angle2 to go from 0 to 45

    setServo(0, angle);
    setServo(6, angle);

    setServo(2, angle2); // set front-left hip from 0 to 45
    setServo(4, angle2); // set back-right hip from 0 to 45

    delay(20);
  }

  int position3 = 55;
  setMultipleServos(KNEE_CHANNELS, 4, position3);

  for (int angle = 0; angle <= 45; angle += 5)
  {
    // set front-left hip and back-right hip from 90deg to 45deg (default position)
    setServo(0, angle);
    setServo(6, angle);

    delay(20);
  }
}

void happyAction()
{
  int count = 0;

  while (count <= 4)
  {
    // Move knees simultaneously
    for (int angle = 0; angle <= 45; angle += 5)
    {
      // Calculate pulse lengths for each group
      int pulseLength1 = calculatePulseLength(45 - angle); // For front left and back right (45 to 0)
      int pulseLength2 = calculatePulseLength(45 + angle); // For front right and back left (45 to 90)

      // Set PWM for the first group
      pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, pulseLength1); // Front Left Knee
      pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, pulseLength1); // Back Right Knee

      // Set PWM for the second group
      pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, pulseLength2); // Front Right Knee
      pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, pulseLength2);   // Back Left Knee

      delay(20); // Smooth delay between movements
    }

    // Reverse the movement
    for (int angle = 45; angle >= 0; angle -= 5)
    {
      // Calculate pulse lengths for each group
      int pulseLength1 = calculatePulseLength(45 - angle); // For front left and back right (0 to 45)
      int pulseLength2 = calculatePulseLength(45 + angle); // For front right and back left (90 to 45)

      // Set PWM for the first group
      pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, pulseLength1); // Front Left Knee
      pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, pulseLength1); // Back Right Knee

      // Set PWM for the second group
      pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, pulseLength2); // Front Right Knee
      pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, pulseLength2);   // Back Left Knee

      delay(20); // Smooth delay between movements
    }

    count++;
  }
}

void moveForward()
{
  // Step 1: back-left knee rising
  int kneePosition = 55;
  setServo(3, kneePosition);
  // setServo(5, kneePosition);

  // move back-left hip from 45 to 0 deg
  for (int angle = 45; angle >= 0; angle -= 5)
  {
    setServo(4, angle);

    // Calculate angle2 to go from 45 to 60
    int angle2 = 45 + ((45 - angle) * (70 - 45)) / 45;

    // Set another servo (e.g., servo 6) to angle2
    setServo(5, angle2);

    delay(20);
  }

  // standing back-left knee
  kneePosition = 35;
  setServo(5, kneePosition);
  setServo(3, kneePosition);

  setServo(7, 35);

  for (int angle = 45; angle >= 0; angle -= 5)
  {
    setServo(2, angle);

    // Calculate angle2 going from 0 to 45
    int angle2 = 45 - angle;
    setServo(4, angle2);

    // Calculate angle3 going from 45 to 90
    int angle3 = 90 - angle;
    setServo(6, angle3);

    delay(20);
  }

  // raise front right knee
  kneePosition = 55;
  setServo(3, kneePosition);
  setServo(5, kneePosition);
  setServo(7, 45);

  for (int angle = 0; angle <= 45; angle += 5)
  {
    setServo(2, angle);

    delay(20);
  }

  setServo(1, 55);
  setServo(7, 55);

  for (int angle = 45; angle <= 90; angle += 5)
  {
    setServo(0, angle);

    // Calculate angle2 going from 90 to 45
    int angle2 = 135 - angle;
    setServo(6, angle2);
    delay(20);
  }
};

void defenseAction()
{
  for (int angle = 45; angle <= 60; angle++)
  {
    setServo(3, angle);
    setServo(5, angle);

    int angle2 = 90 - angle;
    setServo(1, angle2);
    setServo(7, angle2);

    delay(20);
  }

  for (int angle = 60; angle >= 30; angle--)
  {
    setServo(3, angle);
    setServo(5, angle);

    int angle2 = 90 - angle;
    setServo(1, angle2);
    setServo(7, angle2);

    delay(20);
  }

  for (int angle = 45; angle <= 90; angle += 2)
  {
    setServo(2, angle);
    setServo(4, angle);

    int angle2 = 90 - angle;
    setServo(0, angle2);
    setServo(6, angle2);

    delay(20);
  }

  delay(1000);
}

void pullUpAction()
{
  int backLeftKneesPos = 35;
  int backRightKneePos = 55;

  setServo(5, backLeftKneesPos);
  setServo(7, backRightKneePos);

  for (int angle = 45; angle <= 90; angle += 2)
  {
    setServo(4, angle); // move back-left hip from 45 to 90

    int angle2 = 90 - angle; // angle2 goes from 45 to 0
    setServo(6, angle2);     // move back-right hip from 45 to 0
    delay(20);
  }

  int pullUpCount = 0;

  // do 3 pull up
  while (pullUpCount <= 3)
  {
    for (int angle = 45; angle >= 10; angle -= 2)
    {
      setServo(1, angle); // set front-left knee goes from 45 to 10

      int angle2 = 90 - angle; // Calculate angle2 to go from 45 to 80
      setServo(3, angle2);     // set front-right knee goes from 45 to 80

      delay(20);
    }

    for (int angle = 10; angle <= 45; angle += 2)
    {
      setServo(1, angle); // set front-left knee goes from 10 to 45

      int angle2 = 90 - angle; // Calculate angle2 to go from 80 to 45
      setServo(3, angle2);     // set front-right knee goes from 80 to 45

      delay(20);
    }

    delay(10);

    pullUpCount++;
  }
}
