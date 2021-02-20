// Motor encoder output pulse per rotation (change as required)
#define ENC_COUNT_REV 460

//Encoder output to Arduino Interrupt pin, connected to channel A
#define ENC_IN 3 
#define ENC_IN2 6

//CCW connected to pin 10
#define CCW 12 
//CW connected to pin 12
#define CW 13 



// Analog pin for potentiometer
int speedcontrol = 0;

// Pulse count from encoder
volatile long encoderValue = 0;

// One-second interval for measurements, measured in milliseconds
int interval = 1000; 

// Counters for milliseconds during interval
long previousMillis = 0;
long currentMillis = 0;

// Variable for RPM measurement
int rpm = 0;

// Variable for PWM motor speed output
int motorPwm = 0;

void setup()
{
  // Setup Serial Monitor
  Serial.begin(9600); 
  
  // Set encoder as input with internal pullup  
  pinMode(ENC_IN, INPUT_PULLUP);
  pinMode(ENC_IN2, INPUT_PULLUP);

  // Set CCW and CW connections as outputs
  pinMode(CCW, OUTPUT);
  pinMode(CW, OUTPUT);
  
  // Attach interrupt 
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);
  
  // Setup initial values for timer, calculates the # of milliseconds that elapsed from last reset
  previousMillis = millis();
}

void loop()
{
	//Control motor with potentiometer
    //motorPwm = map(analogRead(speedcontrol), 0, 1023, 0, 255);
  	int raw = analogRead(speedcontrol);
  
  	if(raw >512){
       	motorPwm = map(raw,512,1023,0,255);
  		analogWrite(CW, motorPwm);
    }
  	else{
      	motorPwm = map(raw,512,0,0,255);
  		analogWrite(CCW, motorPwm);
    }
      

    // Write PWM to controller
    

  // Update RPM value every second
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;


    // Calculate RPM
    rpm = (encoderValue * 60 / ENC_COUNT_REV);

    // Only update display when there is a reading
    if (motorPwm > 0 || rpm > 0) {
      Serial.print("PWM VALUE: ");
      Serial.print(motorPwm);
      Serial.print('\t');
      Serial.print(" PULSES: ");
      Serial.print(encoderValue);
      Serial.print('\t');
      Serial.print(" SPEED: ");
      Serial.print(rpm);
      Serial.println(" RPM");
      //Serial.print("\ channel A: ");
      //Serial.println(digitalRead(ENC_IN));
      //Serial.print("\ channel B: ");
      //Serial.println(digitalRead(ENC_IN2));

    }
    
    if(digitalRead(ENC_IN) != digitalRead(ENC_IN2)){
    Serial.println("Turning clockwise");
    }
    else{
      Serial.println("Turning counterclockwise");
    }
    encoderValue = 0;
  }
}

void updateEncoder()
{
  // Increment value for each pulse from encoder
  encoderValue++;
  
}
