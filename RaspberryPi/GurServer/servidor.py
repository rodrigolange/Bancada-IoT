# main.py

import threading
import time
import csv
import math
import numpy
from datetime import datetime

from http.server import HTTPServer, BaseHTTPRequestHandler
from yr_fetcher import YRFetcher
from ChirpStackHandler import ChirpStackHandler

# para publicar na AWS IoT Core
from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder


import VariaveisCompartilhadas
VariaveisCompartilhadas.dadosRecebidos = 0
VariaveisCompartilhadas.mensagensDesejadasIntervalo = 5

mqttClient = mqtt_connection_builder.mtls_from_path(
    endpoint="ayskvuv5aj6jn-ats.iot.eu-central-1.amazonaws.com",
    cert_filepath="certs/Lange-gurgame-server-certificate.pem.crt",
    pri_key_filepath="certs/Lange-gurgame-server-private.pem.key",
    client_id="Lange-gurgame-server",
    ca_filepath="certs/AmazonRootCA1.pem",
    clean_session=False,
    keep_alive_secs=30,
)

TOPIC = "lange-gurgame-server/out/#"
file_path = "./dadosIntervalo.csv"

lat, lon = -28.6532, -53.1135  # Coordinates for Ibiruba
nroHoras = 48

forecast = YRFetcher(nroHoras, lat, lon)
listaTransmissoesNodos = {}

def on_message_received(topic, payload, **kwargs):
    if payload != VariaveisCompartilhadas.payloadRecebido:
        print(f"Message on '{topic}': {payload}")
        with VariaveisCompartilhadas.lockDadosRecebidos:
            VariaveisCompartilhadas.dadosRecebidos +=1
        print("*********************************************************************")
        print("*********************************************************************")
        print("recebi via mqtt e incrementei")
        print("mensagens recebidas: %s" %VariaveisCompartilhadas.dadosRecebidos)
        print("*********************************************************************")
        print("*********************************************************************")
        VariaveisCompartilhadas.payloadRecebido = payload
    else:
        print("payloads iguais, descartando")


def subscribe_to_topic():
    print(f"Subscribing to topic '{TOPIC}'...")
    mqttClient.subscribe(
        topic="lange-gurgame-server/#",
        qos=mqtt.QoS.AT_LEAST_ONCE,
        callback=on_message_received,
    )
    print("subscribe ok")


# Define a periodic task to update the weather forecast every hour
def weather_forecast_task():
    while True:
        forecast.fetch_forecast()
        precipitacao = forecast.get_total_precipitation()
        if precipitacao < 1.0:
            VariaveisCompartilhadas.mensagensDesejadasIntervalo = 5
        elif 1.0 <= precipitacao <= 5.0:
            VariaveisCompartilhadas.mensagensDesejadasIntervalo = 10
        else:
            VariaveisCompartilhadas.mensagensDesejadasIntervalo = 15

        print(f"Weather forecast updated. Previsao de precipitacao = {precipitacao}, mensagens desejadas = {VariaveisCompartilhadas.mensagensDesejadasIntervalo}")
        time.sleep(5 * 60)  # Update every hour


def handler_factory(*args, **kwargs):
    return ChirpStackHandler(forecast, listaTransmissoesNodos, VariaveisCompartilhadas, mqttClient, *args, **kwargs)

def reset_counter_periodically():
    interval = 600
    while True:
        time.sleep(interval)
        with VariaveisCompartilhadas.lockDadosRecebidos:
            for _ in range(10):
                 print("--")
            print(f"Mensagens recebidas no intervalo de {interval}s: {VariaveisCompartilhadas.dadosRecebidos}. Desejado: { VariaveisCompartilhadas.mensagensDesejadasIntervalo}")
            timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            with open(file_path, mode='a', newline='') as file:
                writer = csv.writer(file)
                writer.writerow([timestamp, str(VariaveisCompartilhadas.mensagensDesejadasIntervalo),
                                 str(VariaveisCompartilhadas.dadosRecebidos),
                                 str(VariaveisCompartilhadas.indiceSatisfacao)])
                #file.write(str(VariaveisCompartilhadas.dadosRecebidos) + "\n")
                rec = VariaveisCompartilhadas.dadosRecebidos
                desejados = VariaveisCompartilhadas.mensagensDesejadasIntervalo
                indiceSats = (20 + 80 * numpy.exp(-0.002 * (desejados-rec)**2))

                if rec-desejados < 0:
                    indiceSats = indiceSats * -1

                VariaveisCompartilhadas.indiceSatisfacao = indiceSats
                VariaveisCompartilhadas.dadosRecebidos = 0
                print(f"Dados recebidos reset. Novo indice de satisfacao:{VariaveisCompartilhadas.indiceSatisfacao}")
            for _ in range(10):
                print("--")

def main():

    print("Iniciando gurserver - lado servidor")

    # thread de reset do contador de mensagens recebidas
    reset_thread = threading.Thread(target=reset_counter_periodically)  # 300 segundos
    reset_thread.start()

    print("Connecting to AWS IoT Core...")
    connect_future = mqttClient.connect()
    connect_future.result()  # Wait for connection to be established
    print("Connected to AWS IoT Core!")

    print("vou iniciar o subscribe")
    subscribe_to_topic()
    print("iniciado")

    # Start the periodic weather forecast task in a separate thread
    weather_thread = threading.Thread(target=weather_forecast_task)
    weather_thread.daemon = True
    weather_thread.start()

    server_address = ('', 5000)  # Listen on all network interfaces, port 8080
    httpd = HTTPServer(server_address, handler_factory)
#    print("HTTP server running on port 5000...")
    httpd.serve_forever()


if __name__ == "__main__":
    main()
