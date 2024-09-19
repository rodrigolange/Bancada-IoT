
const express = require('express');
const bodyParser = require('body-parser');
const mqtt = require('mqtt');
const fs = require('fs');
const path = require('path');

let previsao48h = 0;

/******************************************
 * Definicoes para o cliente MQTT         *
 ******************************************/
const AWS_IOT_ENDPOINT = 'ayskvuv5aj6jn-ats.iot.eu-central-1.amazonaws.com';
const KEY_PATH = path.join(__dirname, 'certs', 'Lange-BancadaIoT-NodeRed-private.pem.key');
const CERT_PATH = path.join(__dirname, 'certs', 'Lange-BancadaIoT-NodeRed.crt');
const CA_PATH = path.join(__dirname, 'certs', 'AmazonRootCA1.pem');

const options = {
    host: AWS_IOT_ENDPOINT,
    port: 8883,                    // AWS IoT Core uses port 8883 for secure MQTT
    protocol: 'mqtts',             // Secure MQTT connection using TLS/SSL
    key: fs.readFileSync(KEY_PATH),    // Private key
    cert: fs.readFileSync(CERT_PATH),  // Client certificate
    ca: fs.readFileSync(CA_PATH),      // CA certificate
    rejectUnauthorized: true         // Ensures the server certificate is validated
};

const client = mqtt.connect(options);

// Event handler when the client connects successfully
client.on('connect', () => {
    console.log('Connected to AWS IoT Core');

    // Subscribe to a topic
    client.subscribe('$aws/things/Lange-BancadaIoT-MQTTBridge/yr', (err) => {
        if (err) {
            console.error('Failed to subscribe:', err);
        } else {
            console.log('Subscribed to topic: $aws/things/Lange-BancadaIoT-MQTTBridge/yr');
        }
    });
});


/******************************************************************
 * Web server para interagir com a integracao HTTP do Chirpstack  *
 ******************************************************************/
const app = express();
const port = 5000;

// Middleware to parse JSON data
app.use(bodyParser.json());

// Route to handle uplink data
app.post('/uplink', (req, res) => {
    const data = req.body;
    console.log("Received Uplink:", data.deviceInfo.devEui);
    // Process the uplink data as needed
    res.status(200).json({ status: 'success' });
});

// GET endpoint to fetch the number of previsao acumulada para 48h
app.get('/acumulado', (req, res) => {
    res.json({
        previsao: previsao48h
    });
});

// Route to handle downlink notifications
// ToDo: tirar essa rota fora, nao eh necessaria
// Estah aqui para testes
app.post('/downlink', (req, res) => {
    const data = req.body;
    console.log("Received Downlink:");
    res.status(200).json({ status: 'success' });
});

// Start the server
app.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
