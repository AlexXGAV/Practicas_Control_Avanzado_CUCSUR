// ========= Modelador de respuesta al Impulso Unitario (Función escalón de Heaviside) =========

// Definición de pines
const int TRIAC_PIN = 3;            // Pin de control del TRIAC (salida PWM)
const int AC_DETECT_PIN = 2;        // Pin de detección de cruce por cero en corriente alterna (AC)
const int LM35_PIN = A1;            // Pin de entrada del sensor LM35
const int POTENTIOMETER_PIN = A0;   // Pin de entrada del potenciómetro

// Variables
int potValue = 0;                   // Valor del potenciómetro
int triacPower = 0;                 // Valor de potencia del TRIAC
float temperature = 0.0;            // Valor de temperatura del LM35
unsigned long startTime = 0;        // Tiempo de inicio de la activación
unsigned long duration = 30000;     // Duración de la activación (en milisegundos)
unsigned long duration_off = 5000;  // Duración de la desactivación (en milisegundos)

unsigned long delayTime = 10;       // Tiempo de espera en void loop()

bool lastState = 0;                 // Para control de histerisis
bool isOn = 0;                      // Para control ON-OFF

// Configuración Inicial (Se ejecuta una sola vez, después de cada encendido o reinicio de la placa)
void setup() {
  pinMode(TRIAC_PIN, OUTPUT);                                       // Configura el pin "TRIAC_PIN" como salida
  pinMode(AC_DETECT_PIN, INPUT);                                    // Configura el pin "AC_DETECT_PIN" como entrada

  Serial.begin(9600);                                               // Inicializa el puerto Serial a 9600 baudios
  

  Serial.println("Tiempo (ms),Potencia (%),Temperatura (°C)");      // Imprime los encabezados de las columnas donde se capturaran los datos

  startTime = millis();                                             // Asigna el tiempo de inicio obteniéndolo de la funcion "millis()"

}

// Bucle de ejecución constante (infinito)
void loop() {

  // Control
  onOff();                                                          // Ejecuta la función "onOff"
  //hysterisis();                                                   // Ejecuta la función "hysterisis"

  // Obtención de datos
  sendValuesToSerialPort();                                         // Ejecuta la función "sendValuesToSerialPort"
  //plotValuesInSerialPlot();                                       // Ejecuta la función "plotValuesInSerialPlot"

  // Esperar un tiempo antes de la siguiente lectura
  delay(delayTime);                                                 // Espera el tiempo de la variable "delayTime"
}

// Control ON-OFF
void onOff(){
  if (zeroDetected()){                                              // Si se detecta el cruce por cero

    if ((duration_off < millis() - startTime) && !isOn) {           // Si la duracion de apagado es menor a la diferencia del tiempo acutal y el tiempo inicial, y esta apagado
      isOn = 1;                                                     // Cambia el estado a verdadero (1)
      powerOn();                                                    // Ejecuta la función "powerOn"
      startTime = millis();                                         // Reicinia el tiempo inicial
    }
  }
    
    if((duration < millis() - startTime) && isOn){                  // Si la duracion de encendido es menor a la diferencia del tiempo acutal y el tiempo inicial, y esta encendido
      isOn = 0;                                                     // Cambia el estado a falso (0)
      powerOff();                                                   // Ejecuta la función "powerOff"
      startTime = millis();                                         // Reicinia el tiempo inicial
    }
}

// Control Histerisis
void hysterisis(){
  if (zeroDetected()){                                              // Si se detecta el cruce por cero
    if ((millis() - startTime > duration)) {                        // Si la duracion es menor a la diferencia del tiempo actual con el tiempo inicial
      powerHisterisis();                                            // Ejecuta la función "powerHisterisis"
      startTime = millis();                                         // Reicinia el tiempo inicial
    }
  }
}

// Encendido y apagado de histerisis
void powerHisterisis(){
    potValue = analogRead(POTENTIOMETER_PIN);                       // Leer el valor del potenciómetro
    triacPower = map(potValue, 0, 1023, 0, 255)*!(lastState);       // Mapear el valor del potenciómetro a un rango de 0 a 255 (valor PWM) si el "lastState" es falso
    analogWrite(TRIAC_PIN, triacPower);                             // Aplicar PWM al TRIAC para controlar la potencia
    lastState = !lastState;                                         // Cambia el último estado a su mismo valor contrario
  
}

// Encendido 
void powerOn(){
  potValue = analogRead(POTENTIOMETER_PIN);                         // Leer el valor del potenciómetro
  triacPower = map(potValue, 0, 1023, 0, 255);                      // Mapear el valor del potenciómetro a un rango de 0 a 255 (valor PWM)
  analogWrite(TRIAC_PIN, triacPower);                               // Aplicar PWM al TRIAC para controlar la potencia
}

// Apagado
void powerOff(){
  analogWrite(TRIAC_PIN, 0);                                        // Aplicar PWM al TRIAC para apagar
  //digitalWrite(TRIAC_PIN, 0);                                     // Aplicar control digital para apagar (equivale a escribir 0 con PWM)
}

// Cruce por cero
bool zeroDetected(){
  return digitalRead(AC_DETECT_PIN);                                // Regresa el valor leido digitalmente del cruce por cero (1 o 0)
}

// Obtener temperatura
float getTemperature(){
  return (analogRead(LM35_PIN) / 1023.0) * 5000.0 / 10.0;           // Regresa el valor leido y convertido a la temperatura en grados Celsius del sensor LM35 
}

// Enviar valores al puerto serial para capturarlos en formato csv
void sendValuesToSerialPort(){
  Serial.print(millis());                                           // Tiempo
  Serial.print(",");                                                // , para separar la columna
  Serial.print(map(triacPower, 1, 255, 0, 100));                    // Potencia (convertida de valores PWM a un rango de 0 a 100)
  Serial.print(",");                                                // , para separar la columna
  Serial.println(getTemperature());                                 // Temperatura y termina dando un salto de linea (\n) con ".printIn" para agregar nueva fila
}

// Enviar datos a puerto serial para visualizar en Serial Plot
void plotValuesInSerialPlot(){
  Serial.print("Potencia TRIAC:");                                  // Etiqueta de potencia
  Serial.print(map(triacPower, 0, 255, 0, 100));                    // Valor de potencia
  Serial.print(",Temperatura LM35:");                               // Etiqueta de Temperatura
  Serial.print(getTemperature());                                   // Valor de temperatura
  Serial.println(" °C uT ");                                        // Agrega a etiqueta de temperatura " °C uT ", uT: para ajustar escala de tiempo de la gráfica, salto de linea
}