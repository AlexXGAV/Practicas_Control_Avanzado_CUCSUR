// ========= Dimmer Digital con PWM =========

// Definición de pines
const int TRIAC_PIN = 3;
const int POTENTIOMETER_PIN = A0;
const int  AC_IN_ZERO = 2;

// Configuración Inicial (Se ejecuta una sola vez, después de cada encendido o reinicio de la placa)
void setup() {
  pinMode(TRIAC_PIN, OUTPUT);                                       // Configura el pin "TRIAC_PIN" como salida
  pinMode(AC_IN_ZERO, INPUT);                                       // Configura el pin "AC_DETECT_PIN" como entrada
  Serial.begin(9600);                                               // Inicializa el puerto Serial a 9600 baudios
}

// Bucle de ejecución constante (infinito)
void loop() {

  int zero = digitalRead(AC_IN_ZERO);                               // Cruce por cero

  int lecturaPotenciometro = analogRead(POTENTIOMETER_PIN);         // Valor del potenciometro

  //int correccionPot = (lecturaPotenciometro>0) * 1.0 * exp(0.00677468 * lecturaPotenciometro);
  //lecturaPotenciometro = correccionPot;

  int onTime = map(lecturaPotenciometro, 0, 1023, 0, 7300);         // Mapeo de valor del potenciometro al tiempo
  
  digitalWrite(TRIAC_PIN, (zero) * (lecturaPotenciometro>0));       // Encendido si -> hay cruce por cero y el valor del potenciometro es mayor a 0
  delayMicroseconds((zero) * onTime * (lecturaPotenciometro>0));    // Retardo en microsegundos "onTime" si -> hay cruce por cero y el valor del potenciometro es mayor a 0
  digitalWrite(TRIAC_PIN, LOW);                                     // Apagado

  delay(10);                                                        // Retardo de 10ms

  // Para graficar en Serial Plot (esto agrega un retardo en el control)

  //Serial.print("Z:");
  //Serial.print(zero * 1000);

  //Serial.print(",T(0-7300):");
  //Serial.print((onTime) + 4000);

  //Serial.print(",P(0-1023):");
  //Serial.print(lecturaPotenciometro + 2000);

  //Serial.println("uT ");

}
