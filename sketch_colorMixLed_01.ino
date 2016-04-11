float baselineTemp = 0.0;
//const int sensorPin = A0;    // pin that the sensor is attached to
const int ledPin = 2;        // pin that the LED is attached to

// variables:
int sensorVal = 0;         // the sensor value
int sensorMin = 1023;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value
int CalibSensorVal = 0;

void setup() 
{
  Serial.begin(9600);
  //inintialise all pins / LEDs to off
  for (int pinNumber=2; pinNumber<5; pinNumber++)
  {
    digitalWrite(pinNumber, LOW);
  }
  // turn on LED to signal the start of the calibration period:
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);  
  digitalWrite(4, HIGH);
  // calibrate during the first five seconds
  while (millis() < 5000)
  {
    sensorVal = analogRead(A0);
    // record the maximum sensor value
    if (sensorVal > sensorMax) {      sensorMax = sensorVal;    }
    // record the minimum sensor value
    if (sensorVal < sensorMin)  {     sensorMin = sensorVal;    }
  }
  // apply the calibration to the sensor reading
  sensorVal = map(sensorVal, sensorMin, sensorMax, 0, 255);
  //convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0)*5;
  //convert the voltage to temp
  baselineTemp = (voltage - .5) *100;
  // fade the LED using the calibrated value:
  analogWrite(ledPin, sensorVal);
  // signal the end of the calibration period
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  Serial.println("Calibration complete");
}

//-------------THIS IS THE MAIN LOOP-----------------
void loop()
{
  delay(2000);

  //initialise complete now read
  sensorVal = analogRead(A0);
    // in case the sensor value is outside the range seen during calibration
  sensorVal = constrain(sensorVal, 0, 255);
  
  Serial.print("\nSensor Value is ");
  Serial.print(sensorVal);
  
  //convert the ADC reading to voltage
  float voltage = (sensorVal / 1024.0)*5;
  Serial.print("  Volts : ");
  Serial.print(voltage);

  //convert the voltage to temp
  float temperature = (voltage - .5) *100;
  Serial.print("  degrees C : ");
  Serial.println(temperature);

  if (temperature < baselineTemp)
  {
    Serial.println("lowtemplight");
    
    Serial.print("\nTemp is ");
    Serial.print(temperature);
    Serial.print("  Baseline temp is ");
    Serial.print(baselineTemp);
    for (int i=2;i<5;i++)
    {
      digitalWrite(i,HIGH);
      delay(500);
      digitalWrite(i, LOW);
    }
    
  }
  else if(temperature >= baselineTemp && temperature< (baselineTemp + 2))
  {
    Serial.println("onelight");
    Serial.print("\nTemp is ");
    Serial.print(temperature);
    Serial.print("  Baseline temp is ");
    Serial.print(baselineTemp);
    digitalWrite(2,HIGH);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
  }
  else if(temperature >= (baselineTemp + 2) && temperature < (baselineTemp + 4))
  {
    Serial.print("\nTemp is ");
    Serial.print(temperature);
    Serial.print("  Baseline temp is ");
    Serial.print(baselineTemp);
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,LOW);
  }  
  else if(temperature >= (baselineTemp + 4))
  {
    Serial.print("\nTemp is ");
    Serial.print(temperature);
    Serial.print("  Baseline temp is ");
    Serial.print(baselineTemp);
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);
    digitalWrite(4,HIGH);
  }

  delay(1);
  
}
