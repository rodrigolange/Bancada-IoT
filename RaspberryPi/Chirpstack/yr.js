async function getWeatherForecast(lat, lon) {
    const url = `https://api.met.no/weatherapi/locationforecast/2.0/complete?lat=${lat}&lon=${lon}`;
    const options = {
        headers: {
            'User-Agent': 'Lange-BancadaIoT/1.0 (rodrigo.lange@ibiruba.ifrs.edu.br)'  // Replace with your app name and email
        }
    };

    let totalPrecipitacao = 0;

    try {
        const response = await fetch(url, options);
        if (!response.ok) {
            throw new Error(`Error fetching data: ${response.statusText}`);
        }

        const weatherData = await response.json();
        const timeseries = weatherData.properties.timeseries;

        console.log(url);

        timeseries.forEach((entry) => {

//                  const temperature = entry.data.instant.details.air_temperature; // Temperature
                    if (entry.data?.next_6_hours != undefined){
			 const time = entry.time; // Forecast time
                         const precipitation = entry.data.next_6_hours.details.precipitation_amount;
                         totalPrecipitacao = totalPrecipitacao + precipitation;
                         console.log(`Time: ${time}`);
                         console.log(`Precipitation: ${precipitation} mm`);
                         console.log('---');
                    }
        });
	console.log(`Previsao de precipitacao acumulada: ${totalPrecipitacao} mm`);
    } catch (error) {
        console.error('Error fetching the weather forecast:', error);
    }
}

// Example usage
getWeatherForecast(-28.6532, -53.1135); // Coordinates for Ibiruba

