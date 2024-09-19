const mqtt = require('mqtt');
const fs = require('fs');
const path = require('path');

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
    acumulado48h = jsonObject.state.reported.acumulado48h;
    console.log(acumulado48h);
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
