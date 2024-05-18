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

ToDo: rever os locais das pastas. O comando mv ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/ ~/lorawan/ não precisa. Ajustar.

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

### Mover os arquivos para lugares padrão:

sudo cp reset_lgw.sh /usr/bin/

sudo cp -r ~/lorawan/sx1302_hal/packet_forwarder /opt/lorawan

### Criar o arquivo de inicialização do serviço pktForwarder

Criar o arquivo:

```
sudo nano sudo nano /etc/init.d/lorapktfwd
```

Colocar os comandos de inicialização no arquivo:

```
#!/bin/bash
### BEGIN INIT INFO
# Provides:          lorapkdfwd
# Required-Start:    $all
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:
# Short-Description: Inicia lora packet forwarder
### END INIT INFO

case "$1" in
        start)
                echo "Starting packet forwarder"
                # run the program you want to start
                cd /opt/lorawan/packet_forwarder        # ver essa linha, precisa por causa do reset_lgw.sh
                sudo /opt/lorawan/packet_forwarder/lora_pkt_fwd -c /opt/lorawan/packet_forwarder/global_conf.json &
        ;;
        stop)
                echo "Stopping packet forwarder"
                # end the program you want to stop
                killall lora_pkt_fwd
        ;;
        *)
                echo "Usage: /etc/init.d/lorapktfwd {start|stop}"
                exit 1
                ;;
esac
```

Habilitar/configurar a serial do GPS:

```
sudo usermod -a -G dialout "${USER}"
```

E habilitar a serial no config:

Sudo Raspi-config —> Interfacing options —> serial —> no —> yes

Reboot



Direitos no arquivo:

```
sudo chmod +x /etc/init.d/lorapktfwd
```

Instalar o serviço:

```
sudo update-rc.d lorapktfwd defaults
```

Para ver o log:

```
sudo journalctl -f -n 100 -u lorapktfwd.service
```


## Chirpstack:

Tutorial em https://www.chirpstack.io/docs/getting-started/debian-ubuntu.html




