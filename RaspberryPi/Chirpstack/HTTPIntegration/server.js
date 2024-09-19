// Import the required libraries
const express = require('express');
const bodyParser = require('body-parser');
const mqtt = require('mqtt');
const fs = require('fs');
const path = require('path');

let previsao48h = 0;

// AWS IoT Core endpoint (replace with your actual endpoint)
const AWS_IOT_ENDPOINT = 'ayskvuv5aj6jn-ats.iot.eu-central-1.amazonaws.com';

// Paths to the certificate files
const KEY_PATH = path.join(__dirname, 'certs', 'Lange-BancadaIoT-NodeRed-private.pem.key');
const CERT_PATH = path.join(__dirname, 'certs', 'Lange-BancadaIoT-NodeRed.crt');
const CA_PATH = path.join(__dirname, 'certs', 'AmazonRootCA1.pem');

// MQTT connection options
const options = {
    host: AWS_IOT_ENDPOINT,
    port: 8883,                    // AWS IoT Core uses port 8883 for secure MQTT
    protocol: 'mqtts',             // Secure MQTT connection using TLS/SSL
    key: fs.readFileSync(KEY_PATH),    // Private key
    cert: fs.readFileSync(CERT_PATH),  // Client certificate
    ca: fs.readFileSync(CA_PATH),      // CA certificate
    rejectUnauthorized: true         // Ensures the server certificate is validated
};

// Create an MQTT client and connect to AWS IoT Core
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

    // Publish a message to the same topic
    //client.publish('my/topic', JSON.stringify({ message: 'Hello from Node.js!' }));
});

// Event handler when a message is received on a subscribed topic
client.on('message', (topic, message) => {
    mensagem = message.toString();
    jsonObject = JSON.parse(mensagem);
    previsao48h = jsonObject.state.reported.previsao48h;
    console.log(previsao48h);
    //console.log(`Message received on ${topic}: ${message.toString()}\n`);
});

// Event handler for errors
client.on('error', (err) => {
    console.error('Connection error:', err);
});

// Event handler when the connection is closed
client.on('close', () => {
    console.log('Connection closed');
});




// Initialize the express app
const app = express();

// Middleware to parse incoming JSON requests
app.use(bodyParser.json());

// GET endpoint to fetch the number of active nodes
app.get('/api/acumulado', (req, res) => {
    res.json({
        previsao: previsao48h
    });
});

// POST endpoint to update the number of active nodes
app.post('/api/update-active-nodes', (req, res) => {
    const { count } = req.body;

    // Validate the input count
    if (typeof count !== 'number' || count < 0) {
        return res.status(400).json({ error: 'Invalid count value' });
    }

    // Update the global active node count
    // activeNodeCount = count;

    res.json({
        message: 'Active node count updated successfully',
        count: previsao48h
    });
});

// Start the server on port 3000
const PORT = process.env.PORT || 3000;
// app.listen(PORT, 'localhost', () => {
app.listen(PORT, () => {
    console.log(`Server is running on port ${PORT}`);
});
