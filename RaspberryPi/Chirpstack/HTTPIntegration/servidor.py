from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
from chirpstack_api import integration
from google.protobuf.json_format import Parse
import time


# Configuracoes MQTT
previsaoPrecipitacao48h = 0
client_id = "lange-integration-server"
endpoint = "ayskvuv5aj6jn-ats.iot.eu-central-1.amazonaws.com"
root_ca_path = "certs/AmazonRootCA1.pem"
private_key_path = "certs/Lange-BancadaIoT-NodeRed-private.pem.key"
certificate_path = "certs/Lange-BancadaIoT-NodeRed.crt"
topic = "$aws/things/Lange-BancadaIoT-MQTTBridge/yr"

# Função de callback
def custom_callback(client, userdata, message):
    print(f"Recebido uma mensagem no tópico {message.topic}: {message.payload.decode('utf-8')}")
    mensagem = message.payload.decode('utf-8')


# Configurando o cliente MQTT
mqtt_client = AWSIoTMQTTClient(client_id)
mqtt_client.configureEndpoint(endpoint, 8883)
mqtt_client.configureCredentials(root_ca_path, private_key_path, certificate_path)

# Configurações adicionais (opcionais)
mqtt_client.configureAutoReconnectBackoffTime(1, 32, 20)
mqtt_client.configureOfflinePublishQueueing(-1)  # Sem limite na fila de mensagens
mqtt_client.configureDrainingFrequency(2)  # Número de mensagens a processar por segundo quando reconectar
mqtt_client.configureConnectDisconnectTimeout(10)  # Timeout de conexão
mqtt_client.configureMQTTOperationTimeout(5)  # Timeout de operações MQTT

# Conectar ao AWS IoT Core
mqtt_client.connect()
print("Conectado ao AWS IoT Core")

# Subscrever ao tópico
mqtt_client.subscribe(topic, 1, custom_callback)
print(f"Subscrevendo ao tópico {topic}")

#======================================================================================
class Handler(BaseHTTPRequestHandler):
    # True -  JSON marshaler
    # False - Protobuf marshaler (binary)
    json = True

    def do_POST(self):
        self.send_response(200)
        self.end_headers()
        query_args = parse_qs(urlparse(self.path).query)

        content_len = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_len)

        if query_args["event"][0] == "up":
            self.up(body)

    def up(self, body):
        up = self.unmarshal(body, integration.UplinkEvent())
        if len(up.data.hex())>0:
            print("Uplink received from: %s with payload: %s" % (up.device_info.dev_eui, up.data.hex()))


    def unmarshal(self, body, pl):
        if self.json:
            return Parse(body, pl)

        pl.ParseFromString(body)
        return pl

httpd = HTTPServer(('', 5000), Handler)
httpd.serve_forever()

# Mantém a conexão aberta para receber mensagens
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Desconectando...")
    mqtt_client.disconnect()
    print("Desconectado")
