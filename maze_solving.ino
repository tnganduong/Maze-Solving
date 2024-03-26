//Mega2560
// external interrupt int.0    int.1    int.2   int.3   int.4   int.5            
// pin                  2         3      21      20      19      18

volatile int leftEnCount = 0;
volatile int rightEnCount = 0;

// Left Motor
int enL = 3;
int inL1 = 4;
int inL2 = 5;

// Right motor
int enR = 9;
int inR1 = 6;
int inR2 = 7;

const int K = 68;  //adjust K for smooth response
const int obstacleThreshold = 50;


//iR SENSOR
int frontLeftIRPin = A0;
int frontRightIRPin = A1;
int leftIRPin = A2;
int rightIRPin = A3;

void setup()
{
  Serial.begin(9600);

  // interrupt # 5, pin 18
  attachInterrupt(5, leftEnISR, CHANGE); // Also LOW, RISING, FALLING

  // interrupt # 4, pin 19
  attachInterrupt(4, rightEnISR, CHANGE); // Also LOW, RISING, FALLING

  	// Set all the motor control pins to outputs
	pinMode(enR, OUTPUT);
	pinMode(enL, OUTPUT);
	pinMode(inR1, OUTPUT);
	pinMode(inR2, OUTPUT);
	pinMode(inL1, OUTPUT);
	pinMode(inL2, OUTPUT);


  //configure the IR pin as an input
  pinMode(frontLeftIRPin, INPUT);
  pinMode(frontRightIRPin, INPUT);
  pinMode(leftIRPin, INPUT);
  pinMode(rightIRPin, INPUT);
	
	// Turn off motors - Initial state

  digitalWrite(enR, HIGH);
  digitalWrite(enR, HIGH);
	digitalWrite(inR1, HIGH);
	digitalWrite(inR2, HIGH);
	digitalWrite(inL1, HIGH);
	digitalWrite(inL2, HIGH);

}

void loop() {
  int frontLeftIRValue = digitalRead(frontLeftIRPin);
  int frontRightIRValue = digitalRead(frontRightIRPin);
  int leftIRValue = digitalRead(leftIRPin);
  int rightIRValue = digitalRead(rightIRPin);


  // Check obstacle detection in the front

    if (leftIRValue == LOW && frontLeftIRValue && frontRightIRValue == LOW){
    // Obstacle on the left, turn right
    turnRight();}
    else if (rightIRValue == LOW && frontLeftIRValue && frontRightIRValue == LOW){
    // Obstacle on the right, turn left
    turnLeft();}
    else if (leftIRValue == LOW && rightIRValue == LOW){
    // Obstacle detected in the front, turn right
    goForward(100);
  }
}
void goForward(int speed) {
  // Reset encoder counter
  rightEnCount = 0;
  leftEnCount = 0;

	// For PWM maximum possible values are 0 to 255
	analogWrite(enR, speed);

  int motor_L_speed = speed + K*(rightEnCount-leftEnCount);  
  analogWrite(enL, motor_L_speed);

	// Turn on motor A & B
	digitalWrite(inL1, HIGH);
	digitalWrite(inL2, LOW);
	digitalWrite(inR1, HIGH);
	digitalWrite(inR2, LOW);
}

void stop() {
	// Turn off motors 
	digitalWrite(inR1, LOW);
	digitalWrite(inR2, LOW);
	digitalWrite(inL1, LOW);
	digitalWrite(inL2, LOW);
}


void turnRight() {
  // Reset encoder counter
  rightEnCount = 0;
  leftEnCount = 0;

  int speed = 100;
  const int turnWeight = 2;
	analogWrite(enR, speed);

  int motor_L_speed = turnWeight*speed + K*(turnWeight*rightEnCount-leftEnCount);  
  analogWrite(enL, motor_L_speed);

	// Turn on motor A & B
	digitalWrite(inL1, LOW);
	digitalWrite(inL2, LOW);
	digitalWrite(inR1, HIGH);
	digitalWrite(inR2, LOW);  
}

void turnLeft() {
  // Reset encoder counter
  rightEnCount = 0;
  leftEnCount = 0;

  int speed = 100;
  const int turnWeight = 2;
	analogWrite(enL, speed);

  int motor_R_speed = turnWeight*speed + K*(turnWeight*leftEnCount-rightEnCount);  
  analogWrite(enR, motor_R_speed);

	// Turn on motor A & B
	digitalWrite(inL1, LOW);
	digitalWrite(inL2, HIGH);
	digitalWrite(inR1, HIGH);
	digitalWrite(inR2, LOW);  
}

void leftEnISR() {
  leftEnCount++;
}

void rightEnISR() {
  rightEnCount++;
}