#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


/**************************************************
 *                   EmonLib                      *
 **************************************************/
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

#define HOME_VOLTAGE 220.0
#define ADC_BITS    10
#define ADC_COUNTS  (1<<ADC_BITS)
static int amostrasADC                    = 1000;
static int AnalogIn                       = 35;
static unsigned long periodoAtualizacao   = 10000;
float Current;

unsigned long intervaloPubTemp      = 600000;
unsigned long intervaloReboot       = 3600000;   // reboot a cada hora

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/**************************************************
 *                  watch dog                     *
 **************************************************/
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 60                            // 60 segundos
/**************************************************/

/**************************************************
 *                Sensor Temperatura              *
 **************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>

float temperatura;
#define ONE_WIRE_BUS 17
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
/**************************************************/


/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Lange-WiFi"
#define WLAN_PASS       "R0dr1g0L4ng3"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "rodrigolange78"
#define AIO_KEY         "aio_lBxT79jVuiHvJy5SpLtWSaBkqGwe"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish HomeCorrente      = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/DGLange-emonLib3");
Adafruit_MQTT_Publish HomeTemperatura   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/LangeHome-Temperatura");

/*****************************************************************************/
unsigned long time_now        = 0;
unsigned long time_nowTemp    = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AnalogIn, INPUT);   

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  sensors.begin();
  sensors.requestTemperatures();
  temperatura = sensors.getTempCByIndex(0);

  emon1.current(AnalogIn, 50);

  esp_task_wdt_init(WDT_TIMEOUT, true);     //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                   //add current thread to WDT watch  
  esp_task_wdt_reset();

  for(int x=0; x<10; x++){
    emon1.calcIrms(amostrasADC);
    esp_task_wdt_reset();
    delay(5000);
  }

}

void loop() {

  ArduinoOTA.handle();

  if(millis() >= time_now){
    time_now += periodoAtualizacao;
    digitalWrite(LED_BUILTIN, HIGH);

    Current     = emon1.calcIrms(amostrasADC)/3;  // Calculate Irms only
    //delay(500);
    //Current           = emon1.calcIrms(amostrasADC);  // Calculate Irms only
  
    MQTT_connect();
    Serial.print("Sending: ");
    Serial.print(Current);
    if (!HomeCorrente.publish(Current)) {
      Serial.println("\tFailed");
      delay(120000);
    }
    Serial.println("\tOK!");
    digitalWrite(LED_BUILTIN, LOW);
  }

  if(millis() > time_nowTemp){
    time_nowTemp += intervaloPubTemp;

    sensors.requestTemperatures();
    temperatura = sensors.getTempCByIndex(0);
    
    if (!HomeTemperatura.publish(temperatura)) {
      Serial.println("\tFailed");
      delay(120000);
    }     
  }

  if(millis() > intervaloReboot){
      delay(120000);
  }

  esp_task_wdt_reset();

}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
