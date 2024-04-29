#include "secrets.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <DS3231.h>
#include <Wire.h>

#include <OneWire.h>
#include <DallasTemperature.h>

/*******************************************
 * Configuracoes ADC                       *
 *******************************************/
const int ADCPin = 34;
float ADCValue = 0;

/*******************************************
 * Configuracoes DS18B20                   *
 *******************************************/
const int oneWireBus = 13;
OneWire oneWire(oneWireBus);
DallasTemperature sensorTemperatura(&oneWire);
float temperatureC = 0;

/*******************************************
 * Configuracoes Sensor Capacitivo Umidade *
 *******************************************/
const int sensorUmidadeCapacitivo = 32;

/******************************************
 * Configuracoes RTC                      *
 ******************************************/
#define FREQ_ENVIO_DADOS 900UL // 15 min em seconds, characterized as unsigned long
DS3231 myRTC;
byte alarmDay;
byte alarmHour;
byte alarmMinute;
byte alarmSecond;
byte alarmBits;
bool alarmDayIsDay;
bool alarmH12;
bool alarmPM;

/******************************************
 * Configuracoes AWS                      *
 ******************************************/
// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "$aws/things/lange-IoT-SensorPainelSolar/shadow/update/documents"
#define AWS_IOT_SUBSCRIBE_TOPIC "$aws/things/lange-IoT-SensorPainelSolar/shadow/update/documents"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(15000);

void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("Lange-IoT", "senha123");

  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK!");

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint stored in secrets.h
  client.begin(AWS_IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT...");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("OK!");

  if(!client.connected()){
    Serial.println("\nAWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("\nAWS IoT Connected!\n");
}

void messageHandler(String &topic, String &payload) {
  //Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void lwMQTTErr(lwmqtt_err_t reason){
  if (reason == lwmqtt_err_t::LWMQTT_SUCCESS)
    Serial.print("Success");
  else if (reason == lwmqtt_err_t::LWMQTT_BUFFER_TOO_SHORT)
    Serial.print("Buffer too short");
  else if (reason == lwmqtt_err_t::LWMQTT_VARNUM_OVERFLOW)
    Serial.print("Varnum overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_CONNECT)
    Serial.print("Network failed connect");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_TIMEOUT)
    Serial.print("Network timeout");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_READ)
    Serial.print("Network failed read");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_WRITE)
    Serial.print("Network failed write");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_OVERFLOW)
    Serial.print("Remaining length overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_MISMATCH)
    Serial.print("Remaining length mismatch");
  else if (reason == lwmqtt_err_t::LWMQTT_MISSING_OR_WRONG_PACKET)
    Serial.print("Missing or wrong packet");
  else if (reason == lwmqtt_err_t::LWMQTT_CONNECTION_DENIED)
    Serial.print("Connection denied");
  else if (reason == lwmqtt_err_t::LWMQTT_FAILED_SUBSCRIPTION)
    Serial.print("Failed subscription");
  else if (reason == lwmqtt_err_t::LWMQTT_SUBACK_ARRAY_OVERFLOW)
    Serial.print("Suback array overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_PONG_TIMEOUT)
    Serial.print("Pong timeout");
}

void lwMQTTErrConnection(lwmqtt_return_code_t reason){
  if (reason == lwmqtt_return_code_t::LWMQTT_CONNECTION_ACCEPTED)
    Serial.print("Connection Accepted");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNACCEPTABLE_PROTOCOL)
    Serial.print("Unacceptable Protocol");
  else if (reason == lwmqtt_return_code_t::LWMQTT_IDENTIFIER_REJECTED)
    Serial.print("Identifier Rejected");
  else if (reason == lwmqtt_return_code_t::LWMQTT_SERVER_UNAVAILABLE)
    Serial.print("Server Unavailable");
  else if (reason == lwmqtt_return_code_t::LWMQTT_BAD_USERNAME_OR_PASSWORD)
    Serial.print("Bad UserName/Password");
  else if (reason == lwmqtt_return_code_t::LWMQTT_NOT_AUTHORIZED)
    Serial.print("Not Authorized");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNKNOWN_RETURN_CODE)
    Serial.print("Unknown Return Code");
}

void setupRTC(){
  myRTC.setClockMode(false);
  myRTC.setEpoch(1704058461);
  alarmDay = myRTC.getDate();
  alarmHour = myRTC.getHour(alarmH12, alarmPM);
  alarmMinute = myRTC.getMinute();
  alarmSecond = FREQ_ENVIO_DADOS;                           // initialize to the interval length
  alarmBits = 0b00001100;                                   // Alarm 1 when the minutes match
  alarmDayIsDay = false;

  // Upload initial parameters of Alarm 1
  myRTC.turnOffAlarm(1);
  myRTC.setA1Time(
     alarmDay, alarmHour, alarmMinute, alarmSecond,
     alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  
  //myRTC.checkIfAlarm(1);                                    // clear Alarm 1 flag after setting the alarm time
  myRTC.turnOnAlarm(1);                                     // now it is safe to enable interrupt output

  alarmMinute = 0xFF;                                       // a value that will never match the time
  alarmBits = 0b01100000;                                   // Alarm 2 when minutes match, i.e., never
  
  myRTC.setA2Time(                                          // Upload the parameters to prevent Alarm 2 entirely
      alarmDay, alarmHour, alarmMinute,
      alarmBits, alarmDayIsDay, alarmH12, alarmPM);
  myRTC.turnOffAlarm(2);                                    // disable Alarm 2 interrupt
  myRTC.checkIfAlarm(2);                                    // clear Alarm 2 flag
  
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(2000);
  Serial.print("setup RTC...");
  setupRTC();
  Serial.println("OK!");
  
  connectAWS();

}

void loop() {

  client.loop();
  ADCValue = analogRead(ADCPin) * (3.3/4096) * 9.0/3.3;

  sensorTemperatura.requestTemperatures(); 
  temperatureC = sensorTemperatura.getTempCByIndex(0);
  
  //String tensao = String(ADCValue);
  String mensagem;
  mensagem = "{\"state\":{\"reported\":";
  mensagem = mensagem + "{\"tensao\": ";
  mensagem = mensagem + ADCValue;
  mensagem = mensagem + ",";
  mensagem = mensagem + "\"temperatura\": ";
  mensagem = mensagem + temperatureC;
  mensagem = mensagem + ",";
  mensagem = mensagem + "\"umidadeSolo\": ";
  mensagem = mensagem + analogRead(sensorUmidadeCapacitivo);
  mensagem = mensagem + "}}}";

  /**************************************
   * ToDo: rever para botar um while    *
   * confirmar se eh o aws ou o wifi    *
   **************************************/
  if(!client.connected()){
    connectAWS();
  }
/*
  else{
    Serial.println("Erro! Cliente AWS nao conectado!!");
    Serial.print("Enviei a mensagem " + mensagem + " com status: "); 
    //Serial.print(mensagem + ", ");
    Serial.print(client.publish(AWS_IOT_PUBLISH_TOPIC, mensagem));
  }
  */

  Serial.print("Enviei a mensagem " + mensagem + " com status: "); 
  Serial.print(client.publish(AWS_IOT_PUBLISH_TOPIC, mensagem));
  Serial.print(" em ");
  
  DateTime alarmDT = RTClib::now();
  Serial.print(alarmDT.hour());
  Serial.print(":");
  Serial.print(alarmDT.minute());
  Serial.print(":");
  Serial.println(alarmDT.second());  

  delay(1000);

  //myRTC.turnOffAlarm(1);                                // apenas desabilita interrupcao
  uint32_t nextAlarm = alarmDT.unixtime();                // Le timestamp atual do RTC 
  nextAlarm += FREQ_ENVIO_DADOS;                          // novo alarme
  alarmDT = DateTime(nextAlarm);                          // update the DateTime with the new timestamp

  myRTC.setA1Time(                                        // upload the new time to Alarm 1
     alarmDT.day(), alarmDT.hour(), alarmDT.minute(), alarmDT.second(),
     alarmBits, alarmDayIsDay, alarmH12, alarmPM);

  delay(10000);
   
  myRTC.turnOnAlarm(1);                                   // enable Alarm 1 interrupts
  myRTC.checkIfAlarm(1);                                  // clear Alarm 1 flag again after enabling interrupts (desliga rele)

  delay(FREQ_ENVIO_DADOS*1000);
  
}
