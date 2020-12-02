#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <MQ2.h>

// Set these to run example.
#define FIREBASE_HOST "tugas-iot-490d0.firebaseio.com"
#define FIREBASE_AUTH "nX64QbwoACURDGyWPFMuoAfnEgiMPEEF1STkdltf"
#define WIFI_SSID "rizal"
#define WIFI_PASSWORD ""

int pinAout = A0;
int lpg_gas, co_gas, smoke_gas;
MQ2 mq2(pinAout);
const int sensorPin = 13; 
 
void koneksiWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //memulai menghubungkan ke wifi router
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); //status saat mengkoneksikan
  }
  Serial.println("Sukses terkoneksi wifi!");
  Serial.println("IP Address:"); //alamat ip lokal
  Serial.println(WiFi.localIP());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  koneksiWifi();
  mq2.begin();
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Serial.println("Berhasil terkoneksi ke Firabase");
  pinMode(sensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  float* values= mq2.read(true); //jika diset "false" tidak akan dimunculkan di serial monitor
  
  lpg_gas = mq2.readLPG();
  co_gas = mq2.readCO();
  smoke_gas = mq2.readSmoke();
  Serial.println(lpg_gas);
  Serial.println(co_gas);
  Serial.println(smoke_gas);
  int bacasensor = digitalRead(sensorPin);
  Serial.println(bacasensor);
 

  Firebase.pushInt("mq2/lpg_gas",lpg_gas);
  Serial.println("pushed: /mq2/lpg_gas/"); 
  Firebase.pushInt("mq2/co_gas",co_gas);
  Serial.println("pushed: /mq2/co_gas/"); 
  Firebase.pushInt("mq2/smoke_gas",smoke_gas);
  Serial.println("pushed: /mq2/smoke_gas/"); 

  if(bacasensor == 1 ) {
    Firebase.setString("Api", "On");
  }else {
     Firebase.setString("Api", "Off");
  }
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
  delay(2000); 
}
