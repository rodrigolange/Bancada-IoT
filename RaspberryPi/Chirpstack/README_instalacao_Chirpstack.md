# Instalação do Chripstack

Baseado em https://www.chirpstack.io/docs/getting-started/debian-ubuntu.html

## Criar base de dados no PostgreSQL:

```
sudo apt install postgresql
sudo -u postgres psql
```

Criar as tabelas e o usuário:

```
create role chirpstack with login password 'chirpstack';
create database chirpstack with owner chirpstack;
\c chirpstack
create extension pg_trgm;
\q
```

## Instalar o repositório do Chirpstack e os pacotes necessários:

```
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1CE2AFD36DBCCA00

sudo echo "deb https://artifacts.chirpstack.io/packages/4.x/deb stable main" | sudo tee /etc/apt/sources.list.d/chirpstack.list

sudo apt update

sudo apt install chirpstack-gateway-bridge chirpstack
```

## Configurar o serviço Chirpstack Bridge:

Arquivo de configuração localizado em 

/etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml

Necessário ajustar a integração com o mosquitto:

```
sudo nano /etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.toml
```

Alterar as linhas da seção integration.mqtt para os valores desejados

```
[integration.mqtt]
  event_topic_template="au915_0/gateway/{{ .GatewayID }}/event/{{ .EventType }}"
  command_topic_template="au915_0/gateway/{{ .GatewayID }}/command/#"
```

**IMPORTANTE:** Remover as demais entradas event_topic_template e command_topic_template

Instalar o serviço de bridge e iniciar:

```
# start chirpstack_gateway on boot
sudo systemctl enable chirpstack-gateway-bridge

# iniciar o servico chirpstack-gateway
sudo service chirpstack-gateway-bridge start
```

Para ver os logs do serviço:

```
sudo journalctl -f -n 100 -u chirpstack-gateway-bridge
```

## Configurar o Chirpstack

Alterar a configuração para usar apenas a frequência da região desejada. Editar o arquivo abaixo e comentar todas as entradas menos AU915 na seção  enabled_regions=[

```
sudo nano /etc/chirpstack/chirpstack.toml
```

Instalar e iniciar o serviço do Chirpstack:

```
# start chirpstack on boot
sudo systemctl enable chirpstack

# start chirpstack
sudo systemctl start chirpstack
```

Logs do Chirpstack:

```
sudo journalctl -f -n 100 -u chirpstack
```

## Para acessar a console do Chirpstack

http://IP_do_Raspberry>:8080

Usuário: admin
Senha: admin
