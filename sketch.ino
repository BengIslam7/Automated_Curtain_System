#include <WiFi.h>
#include "PubSubClient.h"
#include <Wire.h>
#include <RTClib.h>
#include <ESP32Servo.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
char clientId[50];
char stateMessage[50]; 
const int LIGHT_THRESHOLD = 70;
const char* curtainScheduleTopic = "rideaux/rtc";
const char* curtainModeTopic = "rideaux/mode";
const char* curtainCommandTopic = "rideaux/commande";

WiFiClient espClient;
PubSubClient client(espClient);
Servo curtainServo; 

const int pirPin = 12; 
const int ldrPin = 34; 
const int openLedPin = 26; 
const int closeLedPin = 27; 
const int servoPin = 18; 
const int SDA_PIN = 21; 
const int SCL_PIN = 22; 

RTC_DS1307 rtc;

bool curtainState = false; 
int ldrValue = 0; 
int lightPercentage = 0; 
bool manualOverride = false; 

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  pinMode(pirPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(openLedPin, OUTPUT);
  pinMode(closeLedPin, OUTPUT);
  
  curtainServo.attach(servoPin); 
  Wire.begin(SDA_PIN, SCL_PIN); 
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  curtainServo.write(0); 
  digitalWrite(openLedPin, LOW);
  digitalWrite(closeLedPin, LOW);
  strcpy(stateMessage, "Curtains are CLOSED"); 
  publishState(); 
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" connected");
      client.subscribe(curtainCommandTopic);
      client.subscribe(curtainModeTopic);
      client.subscribe(curtainScheduleTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String stMessage;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
  Serial.println();
  
  if (String(topic) == curtainModeTopic) {
    if (stMessage == "manual") {
      manualOverride = true;
      Serial.println("Manual mode activated");
    } else if (stMessage == "auto") {
      manualOverride = false;
      Serial.println("Auto mode activated");
    }
  }
  
 if (String(topic) == curtainScheduleTopic) {
    int targetHour = stMessage.substring(0, 2).toInt();
    int targetMinute = stMessage.substring(3).toInt();
    while (true) {
        DateTime now = rtc.now(); // Obtenir l'heure actuelle

        if (now.hour() == targetHour  && now.minute() == targetMinute) {
            if (!curtainState) {
                curtainServo.write(90); // Ouvrir le rideau
                curtainState = true;
            } else {
                curtainServo.write(0); // Fermer le rideau
                curtainState = false;
            }
            break; 
        }
        delay(1000); 
    }
}
  if (String(topic) == curtainCommandTopic) {
    Serial.print("Changing curtains to ");
    if (manualOverride && stMessage == "open" && !curtainState) {
      Serial.println("open");
      curtainServo.write(90); 
      curtainState = true;
      strcpy(stateMessage, "Curtains are OPEN");
      digitalWrite(openLedPin, HIGH); 
      delay(1000);
      digitalWrite(openLedPin, LOW); 
    }
    if (manualOverride && stMessage == "close" && curtainState) {
      Serial.println("close");
      curtainServo.write(0); 
      curtainState = false;
      strcpy(stateMessage, "Curtains are CLOSED");
      digitalWrite(closeLedPin, HIGH); 
      delay(1000);
      digitalWrite(closeLedPin, LOW); 
    }
    publishState(); 
  }
}

void publishState() {
  client.publish("STATCURTAINS", stateMessage);
  client.publish("LIGHTPERCENTAGE", String(lightPercentage).c_str());
  client.publish("PIRSENSOR", String(digitalRead(pirPin)).c_str());
  client.publish("LDRSENSOR", String(ldrValue).c_str());
}

void loop() {
  delay(10);
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
  
  ldrValue = analogRead(ldrPin);
  lightPercentage = map(ldrValue, 0, 4095, 0, 100);
  
  if (!manualOverride) {
    if (digitalRead(pirPin) == HIGH) {
      if (!curtainState) {
        curtainServo.write(90); 
        curtainState = true;
        digitalWrite(openLedPin, HIGH); 
        delay(5000);
        digitalWrite(openLedPin, LOW); 
        strcpy(stateMessage, "Curtains are OPEN");
      }
    } else {
      if (lightPercentage > LIGHT_THRESHOLD) {
        if (curtainState) {
          curtainServo.write(0); 
          curtainState = false;
          digitalWrite(closeLedPin, HIGH); 
          delay(5000);
          digitalWrite(closeLedPin, LOW); 
          strcpy(stateMessage, "Curtains are CLOSED");
        }
      } else {
        if (!curtainState) {
          curtainServo.write(90); 
          curtainState = true;
          digitalWrite(openLedPin, HIGH); 
          delay(5000);
          digitalWrite(openLedPin, LOW); 
          strcpy(stateMessage,"Curtains are OPEN");
}
}
}
}
publishState();
}