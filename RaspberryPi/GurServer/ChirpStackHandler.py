from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

from chirpstack_api import integration
from google.protobuf.json_format import Parse

from ChirpStackClient import ChirpStackClient

api_server = "10.0.0.30:8080"
api_token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJjaGlycHN0YWNrIiwiaXNzIjoiY2hpcnBzdGFjayIsInN1YiI6IjJjNWVhZ>"

class ChirpStackHandler(BaseHTTPRequestHandler):
    json = True

    def __init__(self, forecast, listaTransmissaoNodos, *args, **kwargs):
        self.forecast = forecast
        self.chirpStackClient = ChirpStackClient(api_server, api_token)
        self.listaNodos = listaTransmissaoNodos
        super().__init__(*args, **kwargs)


    def do_POST(self):
        self.send_response(200)
        self.end_headers()
        query_args = parse_qs(urlparse(self.path).query)
        content_len = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_len)

        if query_args["event"][0] == "up":
            self.up(body)

        elif query_args["event"][0] == "join":
            self.join(body)

    def up(self, body):
        up = self.unmarshal(body, integration.UplinkEvent())
        # aqui testa se tem payload. se nao tiver, ignora
        if len(up.data.hex())>0:
            print("Uplink received from: %s with payload: %s" % (up.device_info.dev_eui, up.data))
            dev_eui = up.device_info.dev_eui

            if dev_eui in self.listaNodos:
                oldValue = self.listaNodos[dev_eui]
                self.listaNodos[dev_eui] = oldValue + 1
                #print("atualizei %s para %s" % (dev_eui, oldValue+1))
            else:
                #print("nova entrada para %s" % (dev_eui))
                self.listaNodos[dev_eui] = 1

            print("%s transmitiu %s vezes" %(dev_eui, self.listaNodos[dev_eui]))

            precipitacao = self.forecast.get_total_precipitation()
            self.chirpStackClient.send_downlink(device_eui=dev_eui, payload=precipitacao)
            print("*********************************************************************")

    def join(self, body):
        join = self.unmarshal(body, integration.JoinEvent())
        #print("Device: %s joined with DevAddr: %s" % (join.device_info.dev_eui, join.dev_addr))

    def unmarshal(self, body, pl):
        if self.json:
            return Parse(body, pl)

        pl.ParseFromString(body)
        return pl
