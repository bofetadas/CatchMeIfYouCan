#include <DmxSimple.h>

#include <SoftwareSerial.h>
#define BTSerial Serial1
#define BAUD 115200
// Arduino Variablen
int counter;
String data;

// Android Variablen
String ON;
String OFF;
String ESCAPE_CHARACTER;
String playerHeadlightBeamViewCurrentX;
String playerHeadlightBeamViewCurrentY;
String randomHeadlightBeamViewCurrentX;
String randomHeadlightBeamViewCurrentY;

// Player - MH DMX Variablen
int kPan1 = 43;
int kPan2 = 0;
int kTilt1 = 45;
int kTilt2 = 0;
int kDimmer = 100;
int kShutter = 30;
int kFarbe = 160;
int kIris = 4;
int kFokus = 0;

// Computer - MH DMX Variablen
int mPan1 = 42;
int mPan2 = 0;
int mTilt1 = 45;
int mTilt2 = 0;
int mDimmer = 50;
int mShutter = 30;
int mFarbe = 80;
int mIris = 4;
int mFokus = 10;

void setup() {
  // Arduino vorbereiten
  Serial.begin(BAUD);
  BTSerial.begin(BAUD);

  // MH am Arduino registieren
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  DmxSimple.usePin(36);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  

  // Player - MH vorbereiten
  DmxSimple.write(1, kPan1); //Pan
  DmxSimple.write(2, kPan2); //Panfeintuning
  DmxSimple.write(3, kTilt1); //Tilt
  DmxSimple.write(4, kTilt2); //Tiltfeintuning
  DmxSimple.write(7, kDimmer); //Dimmer
  DmxSimple.write(8, kShutter); //Shutter
  DmxSimple.write(9, kFarbe); //Farbe
  DmxSimple.write(18, kIris); //Iris
  DmxSimple.write(22, kFokus); //Schärfe

  // Computer - MH vorbereiten
  DmxSimple.write(33, mPan1); //Pan
  DmxSimple.write(34, mPan2); //Panfeintuning
  DmxSimple.write(35, mTilt1); //Tilt
  DmxSimple.write(36, mTilt2); //Tiltfeintuning
  DmxSimple.write(39, mDimmer); //Dimmer
  DmxSimple.write(40, mShutter); //Shutter
  DmxSimple.write(41, mFarbe); //Farbe
  DmxSimple.write(50, mIris); //Iris
  DmxSimple.write(54, mFokus); //Schärfe

  // Die Ein- und Ausschaltwerte für die arduinointerne LED sind im UTF8 Format, da die Bluetooth LE Datenübertragung von Android zum Bluetooth Modul nur Werte in Form eines ByteArrays nach UTF Standard annimmt.
  ON = "111110";  // Ursprünglicher Wert in Android Studio zum Einschalten der LED war "on", dessen Wert in der UTF8 Tabelle 49 ist.
  OFF = "111102102"; // Ursprünglicher Wert in Android Studio zum Ausschalten der LED war "off", dessen Wert in der UTF8 Tabelle 48 ist.
  ESCAPE_CHARACTER = "126"; // Escape Character, der als Tilde "~" zum Bluetooth Modul geschickt wird. Dessen Wert in der UTF8 Tabelle ist 126.
  counter = 0;
}

void loop() {
  
  // In jedem Loop Durchlauf wird exakt 1 Byte empfangen, bestehend aus 2 Zahlen, welches widerum eine Integer Zahl darstellt (zB wenn die Koordinate 713 vom Android Handy geschickt wurde,
  // entspricht das den Bytes [55,49,51]. Wenn nun die erste Koordinate bestehend aus 3-4 Bytes beim Arduino ankommt, gelangt der Code in die if-Abfrage, bei der der Counter = 0 ist.
  // Nachdem die Koordinate vollständig empfangen wurde (also alle 3-4 Bytes empfangen worden sind - abhängig von der Größe der Koordinate), wird als nächstes der ESCAPE CHARACTER empfangen,
  // welcher signalisiert, dass die Koordinate zuende ist und eine neue Koordinate geschickt wird.
  // Dies geschieht weiter unten in der if-Abfrage, ob "data" dem ESCAPE CHARACTER entspricht. Der Counter wird deshalb weitergezählt, sodass die nächste Variable mit ihrer Koordinate belegt wird.
  // Wenn alle 4 Variablen (playerHeadlightBeamViewCurrentX, playerHeadlightBeamViewCurrentY, randomHeadlightBeamViewCurrentX, randomHeadlightBeamViewCurrentY) mit ihrem Wert belegt sind,
  // werden die Werte an das DMX Endgerät weitergeleitet und die Variablen wieder resettet.
  
  if (BTSerial.available()) {
    data = BTSerial.read();
    Serial.println("data: " + data);
    if (data.toInt() <=57 && data.toInt() >= 48){
      if (counter == 0){
        playerHeadlightBeamViewCurrentX += map(data.toInt(), 48,57,0,9);
        data = "";
      }
      else if (counter == 1){
        playerHeadlightBeamViewCurrentY += map(data.toInt(), 48,57,0,9);
        data = "";
      }
      else if (counter == 2){
        randomHeadlightBeamViewCurrentX += map(data.toInt(), 48,57,0,9);
        data = "";
      }
      else if (counter == 3){
        randomHeadlightBeamViewCurrentY += map(data.toInt(), 48,57,0,9);
        data = "";
      }
    }

    Serial.println("playerHeadlightBeamViewCurrentX: " + playerHeadlightBeamViewCurrentX);
    Serial.println("playerHeadlightBeamViewCurrentY: " + playerHeadlightBeamViewCurrentY);
    Serial.println("randomHeadlightBeamViewCurrentX: " + randomHeadlightBeamViewCurrentX);
    Serial.println("randomHeadlightBeamViewCurrentY: " + randomHeadlightBeamViewCurrentY);
    
    if (data.equals(ESCAPE_CHARACTER)){
      Serial.println("ESCAPED");
      data = "";
      counter +=1;
      if (counter > 3){ 
        sendDataToDMX();
        resetCoordinates();
        counter = 0;
      }
    }

   // Code um die BluetoothLED ein und auszuschalten
    /*if (data.equals(ON)){
      digitalWrite(LED_BUILTIN, HIGH);
      data = "";
    }

    else if (data.equals(OFF)){
      digitalWrite(LED_BUILTIN, LOW);
      data = "";
    }*/
  }
  data = "";
}

void resetCoordinates(){
  playerHeadlightBeamViewCurrentX = "";
  playerHeadlightBeamViewCurrentY = "";
  randomHeadlightBeamViewCurrentX = "";
  randomHeadlightBeamViewCurrentY = "";
}

void sendDataToDMX(){
  // Lampe manuell einmal starten
  // Bluetoothwerte auslesen und in Variable schreiben
  
  DmxSimple.write(1, kPan1); //Pan
  DmxSimple.write(2, kPan2); //Panfeintuning
  DmxSimple.write(3, kTilt1); //Tilt
  DmxSimple.write(4,kTilt2); //Tiltfeintuning
  DmxSimple.write(7, kDimmer); //Dimmer
  DmxSimple.write(9, kFarbe); //Farbe

  DmxSimple.write(33, kPan1); //Pan
  DmxSimple.write(34, kPan2); //Panfeintuning
  DmxSimple.write(35, kTilt1); //Tilt
  DmxSimple.write(36,kTilt2); //Tiltfeintuning
  DmxSimple.write(39, kDimmer); //Dimmer
  DmxSimple.write(41, kFarbe); //Farbe

  /*if (data.equals("49")){
      kTilt1 = 130;
      data = "";
     }
    else if (data.equals("48")){
      kTilt1 = 45;
      data = "";
    }*/

    delay(10);
}
