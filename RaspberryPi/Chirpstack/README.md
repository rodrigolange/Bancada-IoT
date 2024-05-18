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

https://github.com/Elecrow-RD/LR1302_loraWAN

```
mkdir ~/lorawan

cd ~/lorawan

git clone https://github.com/Elecrow-RD/LR1302_loraWAN.git

cd LR1302_loraWAN

mv ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/ ~/lorawan/

~/lorawan/

rm -rf LR1302_loraWAN

cd ~/lorawan/sx1302_hal

make

cd packet_forwarder

chmod 755 reset_lgw.sh 
```

copiar o arquivo global_conf.json do repositório para a pasta ~/lorawan/sx1302_hal/packet_forwarder

alterar o arquivo ~/lorawan/sx1302_hal/packet_forwarder/reset_lgw.sh:

```
sudo nano ~/lorawan/sx1302_hal/packet_forwarder
```

Mudar o pino de reset de acordo com a documentação em https://www.elecrow.com/wiki/lr1302-lorawan-hat-for-rpi-prd.html:

```
SX1302_RESET_PIN=17
```

alterar a função reset() para:

```
reset() {
    echo "CoreCell reset through GPIO$SX1302_RESET_PIN..."
    echo "SX1261 reset through GPIO$SX1302_RESET_PIN..."
    echo "CoreCell power enable through GPIO$SX1302_POWER_EN_PIN..."
    echo "CoreCell ADC reset through GPIO$AD5338R_RESET_PIN..."

    # write output for SX1302 CoreCell power_enable and reset
        pinctrl set $SX1302_RESET_PIN op; WAIT_GPIO
        pinctrl set $SX1302_RESET_PIN op dh; WAIT_GPIO
        pinctrl set $SX1302_RESET_PIN op dl; WAIT_GPIO

        pinctrl set $SX1261_RESET_PIN op; WAIT_GPIO
        pinctrl set $SX1261_RESET_PIN op dl; WAIT_GPIO
        pinctrl set $SX1261_RESET_PIN op dh; WAIT_GPIO

}
```

para testar: na pasta ~/lorawan/sx1302_hal/packet_forwarder, executar

```
sudo ./lora_pkt_fwd -c global_conf.json 
```
e ligar um nodo

## Chirpstack:

Tutorial em https://www.chirpstack.io/docs/getting-started/debian-ubuntu.html




