const int  AC_IN_ZERO = 2;   
const int triac = 3;
int pot_value = 0;
boolean state = LOW;

int estado_AC_IN_ZERO = 0;      
int lastButtonState_asc = 0;  

void setup() {

  pinMode(AC_IN_ZERO, INPUT);

  pinMode (triac, OUTPUT);
}

void loop() {
  estado_AC_IN_ZERO = digitalRead(AC_IN_ZERO);

  if (estado_AC_IN_ZERO != lastButtonState_asc) {

    if (estado_AC_IN_ZERO == LOW) {
      state = true;
    }
  }
  lastButtonState_asc  = estado_AC_IN_ZERO;

  pot_value = map(analogRead(A0), 0, 1023, 7300, 0);
  if (state == true)
  {
    delayMicroseconds(pot_value);
    digitalWrite(triac, HIGH);
    delayMicroseconds(10);
    digitalWrite(triac, LOW);
    state = false;
  }

}
