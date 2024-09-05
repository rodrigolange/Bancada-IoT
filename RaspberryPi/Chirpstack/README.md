# Passos para instalação:

## Softwares básicos necessários:
```
sudo apt update

sudo apt upgrade

sudo apt install -y git gcc g++ clang python3 mosquitto mosquitto-clients redis-server redis-tools postgresql apt-transport-https dirmngr

sudo apt autoremove
```

sudo raspi-config -> interfaces -> enable SPI

## Software do concentrador LR1302: 

Baseado em https://github.com/Elecrow-RD/LR1302_loraWAN

Arquivo README_instalacao_concentrador.md.md


## Chirpstack:

Tutorial em https://www.chirpstack.io/docs/getting-started/debian-ubuntu.html

Instalação do Chirpstack no Raspberry Pi: arquivo README_instalacao_Chirpstack.md

Configuração de uma aplicação com gateway: arquivo README_configuracao_Chirpstack.md

Link para exemplo de plugin ADR: https://github.com/Alex9779/Chirpstack-ADR/blob/master/v4/default-custom.js
