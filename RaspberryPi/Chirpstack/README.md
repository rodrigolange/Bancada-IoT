**Passos para instalação:**

**1 - Softwares básicos necessários:**
```
sudo apt update

sudo apt upgrade

sudo apt install -y git gcc g++ clang python3 mosquitto mosquitto-clients redis-server redis-tools postgresql apt-transport-https dirmngr

sudo apt autoremove
```

sudo raspi-config -> interfaces -> enable SPI

**2 - Software do concentrador LR1302:**

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

para testar: na pasta ~/lorawan/sx1302_hal/packet_forwarder, executar

```
sudo ./lora_pkt_fwd -c global_conf.json 
```
e ligar um nodo

**3 - Chirpstack:**

Tutorial em https://www.chirpstack.io/docs/getting-started/debian-ubuntu.html




