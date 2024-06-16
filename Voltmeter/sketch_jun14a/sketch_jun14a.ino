/*
MULTIMETER
 = 5V max
*/

#define vPin 5

float voltage = 0.00;
float volts   = 0.00;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sayHello();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0)
  {
    // Get the voltage
    voltage = analogRead(vPin);
    volts = (voltage / 1023) * 5.0;

    // Converting float to string
    String v = String(volts);

    // Dividing parts of the value
    int n1 = v[0];
    int n2 = v[1];
    int n3 = v[2];

    // Send voltage value to computer
    Serial.write(n1);
    Serial.write(n2);
    Serial.write(n3);
  }
}

void sayHello()
{
  while(Serial.available() <= 0)
  {
    Serial.print('H');        // Send capital H to the computer to say "HELLO!";
    delay(200);
  }
}