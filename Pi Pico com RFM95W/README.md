# Raspberry Pi Pico W

## Tutorial de instalação para Linux ou WSL

Baseado em https://www.raspberrypi.com/news/how-to-blink-an-led-with-raspberry-pi-pico-in-c/

*** Atenção: o Pico W tem o led ligado de forma diferente do Pico 2040 ***
*** Exemplo de blink + serial na pasta de exemplos ***

Passos básicos:

```
sudo apt update

sudo apt full-upgrade

wget -O pico_setup.sh https://rptl.io/pico-setup-script

chmod +x pico_setup.sh

./pico_setup.sh
```

- No WSL2 haverá um erro relacionado ao Code. Ver instruções para instalação do Visual Studio no Windows e no WSL2.

- Como console para o Pico: sugestão para o Windows é utilizar o Thonny (no Linux o Minicom funciona)

Site do Thonny: https://thonny.org/

Instalação do Minicom:

```
sudo apt install minicom
```



*** ---------------------------------------------------------------------------------- ***
*** Informações abaixo precisam ser revistas, ficaram para não perder a informação ***


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


