void setup() {

Serial.begin(9600);
}

void loop() {

float average = 0;
for(int i = 0; i < 1000; i++) {
average = average + (0.0740543262537397 * analogRead(A0) - 37.87878787878788);
delay(1);
}
Serial.print(average);
Serial.println(" mA");
delay(100);
}

