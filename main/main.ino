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

void moveForward(){
  // Step 1: back-left knee rising 
  int kneePosition = 60;
  setServo(3, kneePosition);
  // setServo(5, kneePosition);

  // move back-left hip from 45 to 0 deg
  for(int angle = 45; angle >= 0; angle-=3){
    setServo(4, angle);

    // Calculate angle2 to go from 45 to 60
    int angle2 = 45 + ((45 - angle) * (70 - 45)) / 45;

    // Set another servo (e.g., servo 6) to angle2
    setServo(5, angle2);

    delay(20);
  }

  // standing back-left knee
  kneePosition = 30;
  setServo(5,kneePosition);
  setServo(3, kneePosition);

  setServo(7, 30);

  for(int angle = 45; angle >= 0; angle -=3){
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
  kneePosition = 60;  
  setServo(3, kneePosition);
  setServo(5, kneePosition);
  setServo(7, 45);


  for(int angle = 0; angle <= 45; angle+=3){
    setServo(2, angle);

    delay(20);
  }

  setServo(1, 55);
  setServo(7, 55);

  for(int angle = 45; angle <= 90; angle+=3){
    setServo(0, angle);

    // Calculate angle2 going from 90 to 45
    int angle2 = 135 - angle;   
    setServo(6, angle2);
    delay(20);
  }
  // setServo(1, 45);
  // setServo(3,45);
  // setServo(5, 45);
  // setServo(7,45);

  // for(int angle = 90; angle >= 45; angle--){
  //   setServo(0, angle);
  //   delay(20);
  // }
}

// void moveForward()
// {

// // #define FRONT_LEFT_HIP_CHANNEL 0  // Inner servo for the front-left leg
// // #define FRONT_LEFT_KNEE_CHANNEL 1 // Outer servo for the front-left leg
// // #define FRONT_RIGHT_HIP_CHANNEL 2  // Inner servo for the front-right leg
// // #define FRONT_RIGHT_KNEE_CHANNEL 3 // Outer servo for the front-right leg
// // #define BACK_LEFT_HIP_CHANNEL 4  // Inner servo for the back-left leg
// // #define BACK_LEFT_KNEE_CHANNEL 5 // Outer servo for the back-left leg
// // #define BACK_RIGHT_HIP_CHANNEL 6  // Inner servo for the back-right leg
// // #define BACK_RIGHT_KNEE_CHANNEL 7 // Outer servo for the back-right leg

//   // Step 1: Lifting (front-left & back-right knees)
//   int kneePosition = 30;
//   setServo(1, kneePosition);
//   setServo(7, kneePosition);

//   kneePosition = 20;
//   setServo(3, kneePosition);
//   setServo(5, kneePosition);

//   // Step 2: Stepping (front-left & back-right hips)
//   for (int angle = 45; angle <= 90; angle+=5){
//     //front-left hip goes from 45 to 90
//     setServo(0, angle); 

//     // back-right hip goes from 45 to 0
//     int angle2 = 90 - angle; 
//     setServo(6, angle2); 

//     delay(20);
//   }

//   // Step 3: Landing (front-left & back-right knees)
//   kneePosition = 75;
//   setServo(1, kneePosition);
//   setServo(7, kneePosition);
//   // setMultipleServos(KNEE_CHANNELS, 4, kneePosition);

//   // Step 4: Reset (return to intial states for hips)
//   for (int angle = 90; angle >= 45; angle-=5){
//     //front-left hip goes from 90 to 45
//     setServo(0, angle); 

//     // back-right hip goes from 0 to 45
//     int angle2 = 90 - angle; 
//     setServo(6, angle2); 

//     delay(20);
//   }

//   // Step 5: Lifting (Front-right and back-left knees)
//   kneePosition = 60;
//   setServo(3, kneePosition);
//   setServo(5, kneePosition);
 
//   // kneePosition = 70;
//   // setServo(1, kneePosition);
//   // setServo(7, kneePosition);
  
//   // Step 6: Stepping (Front-right and Back-Left hips)
//   for (int angle = 45; angle <= 90; angle+=5){
//     //front-right hip goes from 45 to 90
//     setServo(2, angle); 

//     // back-left hip goes from 45 to 0
//     int angle2 = 90 - angle; 
//     setServo(4, angle2); 

//     delay(20);
//   }

//   // Step 7: Landing (front-right & back-left knees)
//   kneePosition = 20;
//   setServo(3, kneePosition);
//   setServo(5, kneePosition);
//   // setMultipleServos(KNEE_CHANNELS, 4, kneePosition);

//   // Step 8: Reset (return to intial states for hips, the move done)
//   for (int angle = 90; angle >= 45; angle-=5){
//     //front-right hip goes from 90 to 45
//     setServo(2, angle); 

//     // back-left hip goes from 0 to 45
//     int angle2 = 90 - angle; 
//     setServo(4, angle2); 

//     delay(20);
//   }

// }

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
