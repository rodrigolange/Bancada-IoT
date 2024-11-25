from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder
import threading


dadosRecebidos = 0
indiceSatisfacao = 1
lockDadosRecebidos = threading.Lock()

mensagensDesejadasIntervalo = 5
payloadRecebido = "adf"
