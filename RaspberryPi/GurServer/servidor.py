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

class SilentHTTPRequestHandler(BaseHTTPRequestHandler):
    # Override the log_message method to silence logging
    def log_message(self, format, *args):
        return  # Do nothing, effectively silencing the log messages


def handler_factory(*args, **kwargs):
    return ChirpStackHandler(forecast, listaTransmissoesNodos, *args, **kwargs)


# Modify the HTTPServer to use handler_factory, but with silent logging
class CustomHTTPServer(HTTPServer):
    def __init__(self, server_address, handler_factory):
        self.RequestHandlerClass = handler_factory  # Use the handler_factory for dynamic handlers
        super().__init__(server_address, SilentHTTPRequestHandler)  # Silence logs for all requests


# Set up and start the HTTP server
server_address = ('', 5000)  # Listen on all network interfaces, port 8080
httpd = CustomHTTPServer(server_address, handler_factory)
print("HTTP server running on port 5000...")
httpd.serve_forever()
