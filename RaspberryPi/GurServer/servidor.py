# main.py

import threading
import time

from http.server import HTTPServer, BaseHTTPRequestHandler
from yr_fetcher import YRFetcher
from ChirpStackHandler import ChirpStackHandler


lat, lon = -28.6532, -53.1135  # Coordinates for Ibiruba
forecast = YRFetcher(lat, lon)
listaTransmissoesNodos = {}

# Define a periodic task to update the weather forecast every hour
def weather_forecast_task():
    while True:
        forecast.fetch_forecast()
#        print("Weather forecast updated.")
        time.sleep(5 * 60)  # Update every hour

# Start the periodic weather forecast task in a separate thread
weather_thread = threading.Thread(target=weather_forecast_task)
weather_thread.daemon = True
weather_thread.start()


def handler_factory(*args, **kwargs):
    return ChirpStackHandler(forecast, listaTransmissoesNodos, *args, **kwargs)

# Set up and start the HTTP server
server_address = ('', 5000)  # Listen on all network interfaces, port 8080
httpd = HTTPServer(server_address, handler_factory)
print("HTTP server running on port 5000...")
httpd.serve_forever()
