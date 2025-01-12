
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create an instance of the PCA9685 driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo pulse length range (adjust as needed for your servos)
#define SERVOMIN 150 // Minimum pulse length for 0°
#define SERVOMAX 600 // Maximum pulse length for 180°

#define FRONT_LEFT_HIP_CHANNEL 0  // Inner servo for the front-left leg
#define FRONT_LEFT_KNEE_CHANNEL 1 // Outer servo for the front-left leg

#define FRONT_RIGHT_HIP_CHANNEL 2  // Inner servo for the front-right leg
#define FRONT_RIGHT_KNEE_CHANNEL 3 // Outer servo for the front-right leg

#define BACK_LEFT_HIP_CHANNEL 4  // Inner servo for the back-left leg
#define BACK_LEFT_KNEE_CHANNEL 5 // Outer servo for the back-left leg

#define BACK_RIGHT_HIP_CHANNEL 6  // Inner servo for the back-right leg
#define BACK_RIGHT_KNEE_CHANNEL 7 // Outer servo for the back-right leg

// Map servo numbers to defined servo channels
const int servoChannels[] = {
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

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz for servos

  Serial.println("Servo Control Initialized.");
  Serial.println("Enter command in the format: <servo_number> <angle>");
  Serial.println("Servo Numbers:");
  Serial.println("  0: FRONT_LEFT_HIP");
  Serial.println("  1: FRONT_LEFT_KNEE");
  Serial.println("  2: FRONT_RIGHT_HIP");
  Serial.println("  3: FRONT_RIGHT_KNEE");
  Serial.println("  4: BACK_LEFT_HIP");
  Serial.println("  5: BACK_LEFT_KNEE");
  Serial.println("  6: BACK_RIGHT_HIP");
  Serial.println("  7: BACK_RIGHT_KNEE");
  Serial.println("  8: RESET");

  Serial.println("Example: '1 90' (Move Servo 1 to 90°)");
}

void loop()
{
  // Check if there's input from the Serial Monitor
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n'); // Read input until newline
    input.trim();                                // Remove any extra whitespace

    // Parse the input
    int spaceIndex = input.indexOf(' '); // Find the space between servo number and angle
    if (spaceIndex == -1)
    {
      Serial.println("Invalid input format. Use <servo_number> <angle>.");
      return;
    }

    String servoString = input.substring(0, spaceIndex);  // Extract servo number as string
    String angleString = input.substring(spaceIndex + 1); // Extract angle as string

    // Convert inputs to integers
    int servoNumber = servoString.toInt();
    int angle = angleString.toInt();

    if(servoNumber == 8){
        idle();
    }

    // Validate servo number and angle
    if ((servoNumber >= 0 && servoNumber <= 7) && (angle >= 0 && angle <= 180))
    {
      int channel = servoChannels[servoNumber]; // Map servo number to channel
      int pulseLength = map(angle, 0, 180, SERVOMIN, SERVOMAX);

      // Move the selected servo
      pwm.setPWM(channel, 0, pulseLength);

      // Debug output
      Serial.print("Servo ");
      Serial.print(servoNumber);
      Serial.print(" moved to ");
      Serial.print(angle);
      Serial.println("°");
    }
    else
    {
  
      if (servoNumber < 0 || servoNumber > 7)
      {
        Serial.println("Error: Invalid servo number. Choose a number between 0 and 7.");
      }
      if (angle < 0 || angle > 180)
      {
        Serial.println("Error: Invalid angle. Choose an angle between 0° and 180°.");
      }
      Serial.println("Usage: <servo_number> <angle> (e.g., '1 90').");
    }
  }
}


void idle()
{
  // Default position for knees and hips is 45°
  int defaultPosition = 45;

  setMultipleServos(KNEE_CHANNELS, 4, defaultPosition);
  setMultipleServos(HIP_CHANNELS, 4, defaultPosition);
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

// void turnLeft()
// {

//   int kneePosition = calculatePulseLength(30);

//   pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneePosition);
//   pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneePosition);

//   pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneePosition);
//   pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneePosition);

//   for (int angle = 45; angle <= 90; angle += 5)
//   {
//     int pulseLength = calculatePulseLength(angle); // For front left and back right

//     pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, pulseLength); // Front Left HIP
//     pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, pulseLength); // Back Right HIP
//     delay(20);
//   }

//   int kneePosition2 = calculatePulseLength(60);

//   pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneePosition2);
//   pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneePosition2);

//   pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneePosition2);
//   pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneePosition2);

//   for (int angle = 45; angle <= 90; angle += 5)
//   {
//     int pulseLength1 = calculatePulseLength(angle);       // For front left and back right
//     int pulseLength2 = calculatePulseLength(135 - angle); // For front left and back right (90 to 45)

//     pwm.setPWM(FRONT_RIGHT_HIP_CHANNEL, 0, pulseLength1); // Front Right HIP
//     pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, pulseLength1);   // Back Left HIP

//     pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, pulseLength2); // Front Left HIP
//     pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, pulseLength2); // Back Right HIP
//     delay(20);
//   }

//   int kneePosition3 = calculatePulseLength(35);

//   pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneePosition3);
//   pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneePosition3);

//   pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneePosition3);
//   pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneePosition3);

//   for (int angle = 90; angle >= 45; angle -= 5)
//   {
//     int pulseLength = calculatePulseLength(angle); // For front left and back right

//     pwm.setPWM(FRONT_RIGHT_HIP_CHANNEL, 0, pulseLength); // Front Right HIP
//     pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, pulseLength);   // Back Left HIP
//     delay(20);
//   }
// }