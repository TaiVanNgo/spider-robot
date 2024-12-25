#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create an instance of the PCA9685 driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo pulse length range 
#define SERVOMIN 150 // Minimum pulse length for 0°
#define SERVOMAX 600 // Maximum pulse length for 180°

// Define channels for legs
#define FRONT_LEFT_HIP_CHANNEL 0   // Inner servo for the front-left leg
#define FRONT_LEFT_KNEE_CHANNEL 1  // Outer servo for the front-left leg

#define FRONT_RIGHT_HIP_CHANNEL 2  // Inner servo for the front-right leg
#define FRONT_RIGHT_KNEE_CHANNEL 3 // Outer servo for the front-right leg

#define BACK_LEFT_HIP_CHANNEL 4    // Inner servo for the back-left leg
#define BACK_LEFT_KNEE_CHANNEL 5   // Outer servo for the back-left leg

#define BACK_RIGHT_HIP_CHANNEL 6   // Inner servo for the back-right leg
#define BACK_RIGHT_KNEE_CHANNEL 7  // Outer servo for the back-right leg

void setup() {
  Serial.begin(115200);

  // Initialize PCA9685
  pwm.begin();
  pwm.setPWMFreq(50); // Set frequency to 50 Hz for servos

  // Set initial knee positions to 0° for a standing position
  int kneePulseLength = map(0, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneePulseLength);
  pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneePulseLength);
  pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneePulseLength);
  pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneePulseLength);

  // set default position for spider's hip
  int hipPulseLength1 = map(0, 0, 180, SERVOMIN, SERVOMAX);
  int hipPulseLength2 = map(45, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, hipPulseLength1); //0 deg by default
  pwm.setPWM(FRONT_RIGHT_HIP_CHANNEL, 0, hipPulseLength2); //45 deg by default
  pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, hipPulseLength1); // 0 deg by default
  pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, hipPulseLength2); //45 deg by default


}

void loop() {
  pullUpAction(); // Perform a happy dance
  delay(1000);   // Hold position for 1 second
  idle();        // Return to default state

  delay(2000);   // Wait before the next action
}

void idle() {
  // Default positions for knees (standing position at 0°)
  int kneeDefault = map(0, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, kneeDefault);
  pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, kneeDefault);

  // Default positions for hips
  int hipDefault1 = map(0, 0, 180, SERVOMIN, SERVOMAX);
  int hipDefault2 = map(45, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(FRONT_LEFT_HIP_CHANNEL, 0, hipDefault1); //0 deg by default
  pwm.setPWM(FRONT_RIGHT_HIP_CHANNEL, 0, hipDefault2); //45 deg by default
  pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, hipDefault1); // 0 deg by default
  pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, hipDefault2); //45 deg by default
}

void sayHi(){
  int kneeMove = map(80, 0, 180, SERVOMIN, SERVOMAX);  // Knee lifted position
  int hipMove = map(45, 0, 180, SERVOMIN, SERVOMAX);  // Knee lifted position

}

void moveForward(){
  // Define pulse lengths for movements
  int liftKnee = map(45, 0, 180, SERVOMIN, SERVOMAX);  // Knee lifted position
  int lowerKnee = map(0, 0, 180, SERVOMIN, SERVOMAX);  // Knee default position
}

void pullUpAction(){
  int frontKnee = map(0, 0, 180, SERVOMIN, SERVOMAX);
  int backKnee = map(20, 0, 180, SERVOMIN, SERVOMAX);

  int backHip1 = map(0, 0, 180, SERVOMIN, SERVOMAX);
  int backHip2 = map(45, 0, 180, SERVOMIN, SERVOMAX); 

  pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, frontKnee);
  pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, frontKnee);
  pwm.setPWM(BACK_LEFT_KNEE_CHANNEL, 0, backKnee);
  pwm.setPWM(BACK_RIGHT_KNEE_CHANNEL, 0, backKnee);

  pwm.setPWM(BACK_LEFT_HIP_CHANNEL, 0, backHip2);
  pwm.setPWM(BACK_RIGHT_HIP_CHANNEL, 0, backHip1);

  int pullUpCount = 0;

  // pull up 3 reps
  while(pullUpCount < 3){
    for(int angle = 0; angle <= 45; angle++){
      int pulseLength = map(angle, 0, 180, SERVOMIN, SERVOMAX);

      pwm.setPWM(FRONT_LEFT_KNEE_CHANNEL, 0, pulseLength);
      pwm.setPWM(FRONT_RIGHT_KNEE_CHANNEL, 0, pulseLength);
      
      delay(20);  
    }

    pullUpCount ++;
  }
}

void happyAction() {
  int kneeChannels[] = {FRONT_LEFT_KNEE_CHANNEL, FRONT_RIGHT_KNEE_CHANNEL, BACK_LEFT_KNEE_CHANNEL, BACK_RIGHT_KNEE_CHANNEL};
  
  for (int angle = 0; angle <= 45; angle++) {
    int pulseLength = map(angle, 0, 180, SERVOMIN, SERVOMAX);
    for (int i = 0; i < 4; i++) {
      pwm.setPWM(kneeChannels[i], 0, pulseLength);
    }
    delay(20);
  }
}