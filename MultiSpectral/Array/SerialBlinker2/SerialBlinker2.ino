

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(i + 7, OUTPUT);
    digitalWrite(i + 7, LOW);
  }
  
  Serial.begin(115200, SERIAL_8E2);
  //Serial.begin(9600);
  Serial.println("Top");
}
boolean refresh;
int cur_led = 0;
void loop() {
  while (!Serial.available());
    refresh = true;
    switch(Serial.read()) {
      case 'I': cur_led = 0; break;
      case 'N': cur_led++; cur_led %= 8; break;
      default :refresh = false; break;
    }
    if (refresh) {
      for (int i = 0; i < 7; i++) {
         if (i != cur_led || cur_led == 7) {
           digitalWrite(i + 7,LOW);
         } else {
            digitalWrite(i + 7, HIGH);
         }
      }
    }
    Serial.println(cur_led);
}  
