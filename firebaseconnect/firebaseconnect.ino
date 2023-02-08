#if defined(ESP32)
#include <WiFi.h>
#include "HX711.h"  // LoadCell Library
#include "soc/rtc.h"
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <FlexPress.h>  // FlexPressure Library

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define PRINT_PREC   10
#define PINFLEXPRS_L    36
#define PINFLEXPRS_R    35

#define calibration_factor -231.0
#define cele  2.204623 / -1000
#define DOUT  16
#define CLK   4
/* 1. Define the WiFi credentials */
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY ""

/* 3. Define the RTDB URL */
#define DATABASE_URL "" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL ""
#define USER_PASSWORD ""

//Define Firebase Data object
FirebaseData firebaseData;
FlexPress flexPress;
FirebaseAuth auth;
FirebaseConfig config;
HX711 scale;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;
unsigned long count2 = 0;

void setup()
{
  Serial.begin(115200);
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
}

void loop()
{
  FirebaseJson json;
  FlexPress flexPress;
  float load = scale.get_units()* cele;
  float loadAvg = scale.get_units(10) * cele;
  String loadcell = String(load);
  String loadcellAvg = String(loadAvg, 5);
  String prsL = flexPress.getFlexPressStrL(PRINT_PREC);
  String prsR = flexPress.getFlexPressStrR(PRINT_PREC);
  String prsT = flexPress.getPressStrT(PRINT_PREC);
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    
    if (count == 0) {
      json.set("Scale/" + String(count), loadcell + " kg");
      Serial.println(Firebase.set(firebaseData, F("/balancebag/Sensor/LoadCell"), json));
      json.set("Average/" + String(count), loadcellAvg + " kg");
      Serial.println(Firebase.set(firebaseData, F("/balancebag/Sensor/LoadCell"), json));
      json.set("Left/" + String(count), prsL);
      Serial.println(Firebase.set(firebaseData, F("/balancebag/Sensor/FlexPressure"), json));
      json.set("Right/" + String(count), prsR);
      Serial.println(Firebase.set(firebaseData, F("/balancebag/Sensor/FlexPressure"), json));
      json.set("Top/" + String(count), prsT);
      Serial.println(Firebase.set(firebaseData, F("/balancebag/Sensor/FlexPressure"), json));
    } else {
      json.add(String(count), loadcell + " kg");
      Serial.println(Firebase.updateNode(firebaseData, F("balancebag/Sensor/LoadCell/Scale"), json));
      json.add(String(count), loadcellAvg + " kg");
      Serial.println(Firebase.updateNode(firebaseData, F("balancebag/Sensor/LoadCell/Average"), json));
      json.add(String(count), prsL);
      Serial.println(Firebase.updateNode(firebaseData, F("balancebag/Sensor/FlexPressure/Left"), json));
      json.add(String(count), prsR);
      Serial.println(Firebase.updateNode(firebaseData, F("balancebag/Sensor/FlexPressure/Right"), json));
      json.add(String(count), prsT);
      Serial.println(Firebase.updateNode(firebaseData, F("balancebag/Sensor/FlexPressure/Top"), json));
    }
    count++;
  }
}
