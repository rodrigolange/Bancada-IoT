# Raspberry Pi Pico W

## Tutorial de primeiro uso - No Windows, Platform.io com Visual Studio

https://tutoduino.fr/en/pico-platformio/

### Habilitar long path names no Power Shell do Windows 11 (executar como administrador):

```
New-ItemProperty -Path "HKLM:\SYSTEM\CurrentControlSet\Control\FileSystem" `
-Name "LongPathsEnabled" -Value 1 -PropertyType DWORD -Force
```

### Para o Raspberry Pico W, utilizar as ferramentas fornceicas por maxgerhardt para uso com o Platform.io

https://github.com/maxgerhardt/platform-raspberrypi/tree/develop

Board: rpipicow

Arquivo platformio.ini:

```
[env:rpipicow]
platform = https://github.com/maxgerhardt/platform-raspberrypi.git
board = rpipicow
framework = arduino
board_build.core = earlephilhower
board_build.filesystem_size = 0.5m
```

## LMIC-node

https://github.com/lnlp/LMIC-node

