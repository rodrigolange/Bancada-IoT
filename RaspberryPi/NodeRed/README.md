** Instalação do Raspberry Pi OS

- Seguir as informações em https://www.raspberrypi.com/software/
- Atualizar o sistema

----------------------------------------------------------------

Instalação Node Red no Raspberry Pi, com base na documentação oficial (https://nodered.org/docs/getting-started/raspberrypi)

*** 1. Instalar o node.js. Se possível instalar a versão 18 ou maior:

```
sudo apt update

sudo apt upgrade

sudo apt install build-essential curl git
```

*** 2. No shell do Raspberry OS, executar o script:

```
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

sudo systemctl enable nodered.service

sudo service nodered start
```

Instalar elementos para a interface de usuário:
-> Manage Pallete -> Install -> node-red-dashboard
