/*******************************************************************************
 * 
 *  File:         bsf_pico.h 
 * 
 *                Note: This file cannot be called pico.h due to conflict
 *                      with identical named file in Arduino-embed core.
 * 
 *  Function:     Board Support File for Raspberry Pi Pico
 *                with external SPI LoRa module.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by the MCU).
 *                It supports automatic firmware upload and serial over USB.
 *                No onboard display. Optionally an external display con be connected.
 * 
 *                IMPORTANT information for firmware upload:
 *                ------------------------------------------
 *                Device must be in BOOTSEL mode to upload firmware.
 *                (To put in BOOTSEL mode: press BOOTSEL button, power on or reset board, release BOOTSEL button.)
 *                In platformio.ini:
 *                For Windows specify: upload_protocol = picotool (appears not needed for Mac and Linux).
 *                upload_port is operating system and hardware dependent. 
 *                For convenience, to set upload_port: set upload_port in [pico] section (on top).
 *                Examples:
 *                  Windows: upload_port = E:
 *                  Mac:     upload_port = /Volumes/RPI-RP2
 *                  Linux:   upload_port = /media/<user>/RSPI-RP2
 *                On Windows USB driver for Pico [RP2 Boot (interface 1)] needs be installed with Zadig,
 *                see: https://community.platformio.org/t/official-platformio-arduino-ide-support-for-the-raspberry-pi-pico-is-now-available/20792
 * 
 *                Connect the LoRa module and optional display
 *                according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 * 
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----      
 *                LED   <――――――――――>  25   (LED_BUILTIN) (PIN_LED)
 * 
 *                I2C [display]       GPIO  
 *                ---                 ----
 *                SDA   <――――――――――>  6               (PIN_WIRE_SDA)
 *                SCL   <――――――――――>  7               (PIN_WIRE_SCL)
 *
 *                SPI/LoRa module     GPIO
 *                ---                 ----
 *                SCK   <――――――――――>  2    (SCK)      (PIN_SPI_SCK)
 *                MOSI  <――――――――――>  3    (MOSI)     (PIN_SPI_MOSI)
 *                MISO  <――――――――――>  4    (MISO)     (PIN_SPI_MISO)
 *                NSS   <――――――――――>  5    (SS)       (PIN_SPI_SS)
 *                RST   <――――――――――>  8 
 *                DIO0  <――――――――――>  9
 *                DIO1  <――――――――――>  10
 *                DIO2                -          Not needed for LoRa.
 * 
 ******************************************************************************/

#pragma once

#ifndef BSF_PICO_H_
#define BSF_PICO_H_

#include "LMIC-node.h"

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 5,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 8,
    .dio = { /*dio0*/ 9, /*dio1*/ 10, /*dio2*/ LMIC_UNUSED_PIN }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#endif  // BSF_PICO_H_
