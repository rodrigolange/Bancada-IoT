# weather_forecast.py
import requests
import threading

class YRFetcher:
    def __init__(self, lat, lon):
        self.lat = lat
        self.lon = lon
        self.total_precipitation = 0.0
        self.lock = threading.Lock()

    def fetch_forecast(self):
        url = f"https://api.met.no/weatherapi/locationforecast/2.0/complete?lat={self.lat}&lon={self.lon}"
        headers = {
            'User-Agent': 'Lange-BancadaIoT/1.0 (rodrigo.lange@ibiruba.ifrs.edu.br)'  # Replace with your app name and email
        }

        local_precipitation = 0.0

        try:
            response = requests.get(url, headers=headers)
            response.raise_for_status()

            weather_data = response.json()
            timeseries = weather_data["properties"]["timeseries"]

            for entry in timeseries:
                if 'next_6_hours' in entry["data"]:
                    precipitation = entry["data"]["next_6_hours"]["details"]["precipitation_amount"]
                    local_precipitation += precipitation

            with self.lock:
                self.total_precipitation = round(local_precipitation, 2)

        except requests.RequestException as error:
            print('Error fetching the weather forecast:', error)

    def get_total_precipitation(self):
        with self.lock:
            return self.total_precipitation
