# Configuração de uma aplicação com gateway no Chripstack

## Configurar o Chirpstack para receber dados da bridge/concentrador

### Acessar o console

http://IP_do_Raspberry:8080

Usuário: admin
Senha: admin

Ir em Tenants -> Gateway, botão Add Gateway

![image](https://github.com/rodrigolange/Bancada-IoT/assets/52110364/065295fc-e0dd-42fe-a7cc-4be5c1b80772)


Preencher os dados do gateway. O gateway_ID pode ser obtido no arquivo /opt/lorawan/packet_forwarder/global_conf.json

![image](https://github.com/rodrigolange/Bancada-IoT/assets/52110364/6dba0063-8450-463f-88d5-a33351c13a00)



Após configurado, pode-se testar ligando um nodo e acompanhando em LoRaWAN frames:

![image](https://github.com/rodrigolange/Bancada-IoT/assets/52110364/0353202f-ff0d-4c23-9277-5bedbb6725f6)

