/************************************************************
 * O projeto utiliza a biblioteca MCCI_LoRaWAN_LMIC_library *
 * https://github.com/mcci-catena/arduino-lmic              *
 * Lembrar de alterar o arquivo lmic_project_config.h na    *
 * pasta project_config e alterar a regiao para AU915       *
 *                                                          *
 *                                                          * 
 * string para teste com downlink: base64 aGVsbG8K          *
 *                                                          *
 * struct lmic_t definida em lmic.h                         *
 * Inclui:                                                  *        
 *   - LMIC.frame                                           *
 *   - LMIC.dr                                              *
 ************************************************************/

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "mbedtls/base64.h"

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
unsigned TX_INTERVAL = 300;

//#define ARDUINO_LMIC_CFG_NETWORK_TTN 1

// This EUI must be in little-endian format, so least-significant-byte first. 
static const u1_t PROGMEM APPEUI[8]={ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format
static const u1_t PROGMEM DEVEUI[8]={ 0x07, 0x88, 0x88, 0x00, 0x00, 0x33, 0x32, 0x22 };

void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
static const u1_t PROGMEM APPKEY[16] = { 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

float temperatura = 29.3;
int mydataSize = 8;
int16_t int_temp = (int16_t)(temperatura * 100);

static uint8_t mydata[] = "12Lange\0";

static osjob_t sendjob;
unsigned char stringRecebida[255];
unsigned char *ptr = (unsigned char*)malloc(255);
unsigned int *outlen = 0;
uint8_t currentDataRate;
uint8_t currentTxPower;

/*
void prepare_txDataFrame(){
  mydata[0] = int_temp >> 8;
  mydata[1] = int_temp & 0xFF;
  mydata[2] = 'L';
  mydata[3] = 'a';
  mydata[4] = 'n';
  mydata[5] = 'g';
  mydata[6] = 'e';
  mydata[7] = '\0';  
}
*/

// Pin mapping de acordo com a PCB do projeto
const lmic_pinmap lmic_pins = {
    .nss = 5,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,
    .dio = {26, 35, 34},
};

void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              Serial.print("netid: ");
              Serial.println(netid, DEC);
              Serial.print("devaddr: ");
              Serial.println(devaddr, HEX);
              Serial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  Serial.print("-");
                printHex2(artKey[i]);
              }
              Serial.println("");
              Serial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              Serial.print("-");
                      printHex2(nwkKey[i]);
              }
              Serial.println();
            }
            LMIC_setLinkCheckMode(0);
            break;

        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));

              currentDataRate = LMIC.datarate;
              currentTxPower = LMIC.txpow;
            
              Serial.print("Current Data Rate: ");
              Serial.println(currentDataRate);
              Serial.print("Current Tx Power: ");
              Serial.println(currentTxPower);
            
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));

            if (LMIC.dataLen) {

              Serial.print("Got ");
              Serial.print(LMIC.dataLen);
              Serial.println(" bytes");

              char part[LMIC.dataLen + 1];
              strncpy(part, (const char *)LMIC.frame + LMIC.dataBeg, LMIC.dataLen);
              part[LMIC.dataLen] = '\0';
              Serial.println(part);

              String comando = String(part);

              //size_t dlen = 255;
              //mbedtls_base64_decode(ptr, dlen, outlen, LMIC.frame[LMIC.dataBeg], LMIC.dataLen); //(const unsigned char*)

              /*
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.print(F(" bytes of payload: 0x"));
              for (int i = 0; i < LMIC.dataLen; i++) {
                if (LMIC.frame[LMIC.dataBeg + i] < 0x10) {
                  Serial.print(F("0"));
                }
                Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
              }
              Serial.println();
              */
              if(comando == "aumentar"){   // YXVtZW50YXI=
                Serial.print("Aumentando o periodo. Novo periodo: ");
                TX_INTERVAL = TX_INTERVAL * 2;
                Serial.println(TX_INTERVAL);
              }else if(comando == "diminuir"){ //ZGlt aW51aXI=
                Serial.println("Diminuindo o periodo. Novo periodo: ");
                TX_INTERVAL = TX_INTERVAL / 2;
                Serial.println(TX_INTERVAL);
              }
            }
            
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        //prepare_txDataFrame();
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
    Serial.begin(115200);
    delay(5000);
    Serial.println(F("Starting"));
    delay(1000);
    
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    LMIC_setAdrMode(1);
    //LMIC_setDrTxpow(DR_SF10, 14);  // Set data rate to SF10 and power to 14 dBm


    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);

    delay(5000);
    Serial.println(F("Started"));
}

void loop() {
    os_runloop_once();
}
