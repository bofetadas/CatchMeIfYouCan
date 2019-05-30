#include <SoftwareSerial.h>
#define BTSerial Serial1
#define BAUD 115200
String data;
String ON;
String OFF;

void setup() {
  Serial.begin(BAUD);
  BTSerial.begin(BAUD);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Die Ein- und Ausschaltwerte für die arduinointerne LED sind im UTF8 Format, da die Bluetooth LE Datenübertragung von Android zum Bluetooth Modul nur Werte in Form eines ByteArrays nach UTF Standard annimmt.
  ON = "111110";  // Ursprünglicher Wert in Android Studio zum Einschalten der LED war "on", dessen Wert in der UTF8 Tabelle 49 ist.
  OFF = "111102102"; // Ursprünglicher Wert in Android Studio zum Ausschalten der LED war "off", dessen Wert in der UTF8 Tabelle 48 ist.
}

void loop() {
  // Wenn Daten vom Bluetooth Modul kommen, printe sie im seriellen Monitor und leite sie weiter an die arduinointerne LED
  if (BTSerial.available()) {
    data += BTSerial.read();
    Serial.print(data + " ");
    //Serial.println("");

    if (data.equals(ON)){
      digitalWrite(LED_BUILTIN, HIGH);
      data = "";
    }

    else if (data.equals(OFF)){
      digitalWrite(LED_BUILTIN, LOW);
      data = "";
    }
  }
}