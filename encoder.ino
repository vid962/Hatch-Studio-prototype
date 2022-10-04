  #include <Servo.h>
  Servo myservo; 

  // Pins 2/3 are specific as they are interrupt pins
  int encoderPinSignalA = 2;
  int encoderPinSignalB = 3;
  int encoderPinButton  = 4;

  long lastVolume = 0;
 
  // Volatile variables because our interrupt routine changes these variables
  volatile long volume = 0; 
  volatile int lastEncoded = 0;
 
  int buttonVal,
     lastButtonState = 0;

  void setup() {
   // attaches the servo on pin 9 to the servo object
   myservo.attach(9);                   
  
  // initialize the serial monitor
  Serial.begin (9600);

  // set out input pins as inputs
  pinMode(encoderPinSignalA, INPUT); 
  pinMode(encoderPinSignalB, INPUT);
  pinMode(encoderPinButton, INPUT);

  // enable pullup resistors on interrupt pins
  digitalWrite(encoderPinSignalA, HIGH);
  digitalWrite(encoderPinSignalB, HIGH);

  // call updateEncoder() when any change detected on pin 2/3 interrupt 0/1 pins 
  attachInterrupt(0, serviceEncoderInterrupt, CHANGE); 
  attachInterrupt(1, serviceEncoderInterrupt, CHANGE);
  }

  void loop(){ 
  if(volume != lastVolume) {
    Serial.println(volume);
    lastVolume = volume; // volume changed so update lastVolume. 
  }
  
  buttonVal = digitalRead(encoderPinButton);
  if(buttonVal == LOW && lastButtonState != buttonVal) {
    Serial.println("Button Pressed! ");

    Serial.println(volume);
   
    if(volume<6 && volume>0){
      myservo.write(45);              // I option (1-5)
      delay(1000);   
      myservo.write(90);              
    }
    else if(volume>5 && volume <11){
      myservo.write(45);              // II option (6-10)
      delay(3000);   
      myservo.write(90); 
    }
    else if(volume>10 && volume <16){
      myservo.write(45);              // III option (11-15)
      delay(5000);   
      myservo.write(90); 
    }
     else if(volume>15 && volume <=20){
      myservo.write(45);              // IV option (16-20)
      delay(7000);   
      myservo.write(90); 
    }
    delay(500);
    volume = 0;
  }
  lastButtonState = buttonVal;    

  // Acts as rudimentary software button debouncing 
  delay(60);
}


void serviceEncoderInterrupt() {
  int signalA = digitalRead(encoderPinSignalA);
  int signalB = digitalRead(encoderPinSignalB);

  int encoded = (signalB << 1) | signalA;  // converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

  if((sum == 0b0001 || sum == 0b0111 || sum == 0b1110 || sum == 0b1000) && volume < 20) volume ++;
  if((sum == 0b0010 || sum == 0b1011 || sum == 0b1101 || sum == 0b0100) && volume > 0) volume --;

  lastEncoded = encoded; // store this value for next time
}
