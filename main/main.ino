#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// GPIO A4 → SDA
// GPIO A5  → SCL
// test update new code
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
      // sayHiAction();
      idle(); // Return to idle position after the action
    }
    else if (commandInt == 3)
    {
      Serial.println("Performing Pull up Action...");
      // pullUpAction();
      idle(); // Return to idle position after the action
    }
    else if (commandInt == 4)
    {
      Serial.println("Performing turn left Action...");
      turnLeft();
      idle(); // Return to idle position after the action
    }     
    else if (commandInt == 5)
    {
      Serial.println("Performing turn move forward...");
      moveForward();
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

void setMultipleServos(const int channels[], int numChannels, int deg)
{
  int pulseLength = calculatePulseLength(deg);

  for (int i = 0; i < numChannels; i++)
  {
    pwm.setPWM(channels[i], 0, pulseLength);
  }
}

// void moveServoGroup(const int channels[], int numChannels, int startAngle, int endAngle, int step, int delayMs)
// {

//   for (int angle = startAngle; angle != endAngle; angle += step)
//   {
//     int pulseLength = calculatePulseLength(angle);
//     for (int i = 0; i < numChannels; i++)
//     {
//       pwm.setPWM(channels[i], 0, pulseLength);
//     }
//     delay(delayMs);
//   }
// }

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

// #define FRONT_LEFT_HIP_CHANNEL 0  // Inner servo for the front-left leg
// #define FRONT_LEFT_KNEE_CHANNEL 1 // Outer servo for the front-left leg
// #define FRONT_RIGHT_HIP_CHANNEL 2  // Inner servo for the front-right leg
// #define FRONT_RIGHT_KNEE_CHANNEL 3 // Outer servo for the front-right leg
// #define BACK_LEFT_HIP_CHANNEL 4  // Inner servo for the back-left leg
// #define BACK_LEFT_KNEE_CHANNEL 5 // Outer servo for the back-left leg
// #define BACK_RIGHT_HIP_CHANNEL 6  // Inner servo for the back-right leg
// #define BACK_RIGHT_KNEE_CHANNEL 7 // Outer servo for the back-right leg

  
  setServo(5,60);
  for (int angle = 45; angle >= 20; angle--){
    setServo(4, angle);
  }


delay(1000);


// for (int angle = 45; angle <= 60; angle++)
//   {
//       // Move back-right hip from 45 to 60
//       setServo(6, angle);

//       // Move back-left hip from 45 to 60
//       setServo(4, angle);

//       // Decremental movement for moving from 45 to 20
//       int decrementedAngle = 45 - ((angle - 45) * (45 - 30) / (60 - 45));
//       setServo(2, decrementedAngle); // Move front-right hip from 45 to 20

//       delay(20);
//   }
 

//   int kneePos = 65;
//   setMultipleServos(KNEE_CHANNELS, 4, kneePos);

//   for (int angle = 20; angle <= 45; angle++)
//   {

//     // set front-right hip go from 20 to 45 (default pos)
//     setServo(2, angle); 

//     // Calculate the corresponding angle for the other servos (60 to 45)
//     int decrementedAngle = 60 - ((angle - 20) * (60 - 45) / (45 - 20));
//     setServo(4, decrementedAngle); // Move back-left hip from 60 to 45
//     setServo(6, decrementedAngle); // Move back-right hip from 60 to 45

//     delay(20);
//   }

//   int kneePos2 = 25;
//   setMultipleServos(KNEE_CHANNELS, 4, kneePos2);

//   delay(1000);


// sau khi ve vi tri ban dau thi` set cai knee phai, va doi dien chong len (65)
}

// void sayHiAction()
// {
//   int frontKnees = calculatePulseLength(0);

//   int backKnees = calculatePulseLength(0);

//   int backHipLeft = calculatePulseLength(45);
//   int backHipRight = calculatePulseLength(45);

//   pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, frontKnees);
//   pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, frontKnees);
//   pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, backKnees);
//   pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, backKnees);

//   pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, backHipLeft);
//   pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, backHipRight);

//   // let the back knees down
//   for (int angle = 0; angle <= 45; angle++)
//   {
//     pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//     pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, calculatePulseLength(angle));

//     delay(20);
//   }

//   for (int angle = 45; angle <= 90; angle++)
//   {
//     pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//     delay(20);
//   }

//   for (int angle = 90; angle >= 45; angle--)
//   {
//     pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//     delay(20);
//   }

//   for (int angle = 45; angle <= 90; angle++)
//   {
//     pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//     delay(20);
//   }

//   for (int angle = 45; angle >= 0; angle--)
//   {
//     pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, calculatePulseLength(angle));
//     delay(20);
//   }

//   for (int angle = 0; angle <= 45; angle++)
//   {
//     pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, calculatePulseLength(angle));
//     delay(20);
//   }

// }

// void pullUpAction()
// {
//   int frontKnee = calculatePulseLength(0);
//   int backKnee = calculatePulseLength(0);

//   int backHipLeft = calculatePulseLength(90);
//   int backHipRight = calculatePulseLength(0);

//   pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, frontKnee);
//   pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, frontKnee);
//   pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, backKnee);
//   pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, backKnee);

//   pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, backHipLeft);
//   pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, backHipRight);

//   int pullUpCount = 0;

//   // pull up 3 reps
//   while (pullUpCount < 3)
//   {
//     for (int angle = 0; angle <= 45; angle++)
//     {
//       pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//       pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, calculatePulseLength(angle));

//       delay(20);
//     }

//     for (int angle = 45; angle >= 0; angle--)
//     {
//       pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, calculatePulseLength(angle));
//       pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, calculatePulseLength(angle));

//       delay(20);
//     }

//     pullUpCount++;
//   }
// }
