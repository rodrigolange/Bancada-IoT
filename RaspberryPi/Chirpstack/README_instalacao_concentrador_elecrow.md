# Software do concentrador LR1302: 

https://github.com/Elecrow-RD/LR1302_loraWAN

sudo raspi-config -> interfaces -> enable SPI

```
mkdir ~/lorawan

cd ~/lorawan

git clone https://github.com/Elecrow-RD/LR1302_loraWAN.git

cd ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/

make

```

copiar o arquivo global_conf.json deste repositório para a pasta sx1302_hal/packet_forwarder

```
cd ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/packet_forwarder

wget https://raw.githubusercontent.com/rodrigolange/Bancada-IoT/main/RaspberryPi/Chirpstack/global_conf.json
```


alterar o arquivo reset_lgw.sh:

```
sudo nano ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/packet_forwarder/reset_lgw.sh
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

Direitos de execução no arquivo reset_lgw.sh

```
chmod +x ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/packet_forwarder/reset_lgw.sh
```


para testar: 

```
cd ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/packet_forwarder/

sudo ./lora_pkt_fwd -c global_conf.json 
```
e ligar um nodo

### Mover os arquivos para lugares padrão:

```
sudo mkdir /opt/lorawan

cd ~/lorawan/LR1302_loraWAN/LR1302_HAL/sx1302_hal/

sudo cp -r packet_forwarder/ /opt/lorawan/
```

### Criar o arquivo de inicialização do serviço pktForwarder

Criar o arquivo:

```
/etc/systemd/system/lorapktfwd.service
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
sudo chmod +x /etc/systemd/system/lorapktfwd.service
```

Instalar o serviço:

```
sudo update-rc.d lorapktfwd defaults
```

Iniciar o serviço:

```
sudo service lorapktfwd start
```

Para ver o log:

```
sudo journalctl -f -n 100 -u lorapktfwd.service
```

**Sugestão: desativar o GPS para evitar problemas de inicialização do gateway LoRa**

Editar o arquivo global_conf.json

```
sudo nano /opt/lorawan/packet_forwarder/global_conf.json
```

e comentar as linhas relacionadas ao GPS:

```
"forward_crc_disabled": false //,   <-- comentar essa vírgula para evitar problemas com o json
//"gps_tty_path": "/dev/ttyS0",
//"ref_latitude": 0.0,
//"ref_longitude": 0.0,
//"ref_altitude": 0
```
