Instalação do Raspberry Pi OS

- Seguir as informações em https://www.raspberrypi.com/software/
- Atualizar o sistema

----------------------------------------------------------------

Instalação Node Red no Raspberry Pi, com base na documentação oficial (https://nodered.org/docs/getting-started/raspberrypi)

1. Instalar o node.js. Se possível instalar a versão 18 ou maior.

    sudo apt install nodejs npm build-essential curl git

Verificar a instalação:

    node -v

2. No shell do Raspberry OS, executar o script

    bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)

    sudo systemctl enable nodered.service
    sudo service nodered start

Instalar elementos para a interface de usuário:
-> Manage Pallete -> Install -> node-red-dashboard


3. Adicionais: selenium

https://raspberrypi.stackexchange.com/questions/104002/selenium-wont-run-on-my-raspberry-pi-3-model-b

https://launchpad.net/ubuntu/trusty/armhf/chromium-chromedriver/65.0.3325.181-0ubuntu0.14.04.1
