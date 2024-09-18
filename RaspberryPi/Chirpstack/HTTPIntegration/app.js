var exec = require('child_process').exec;

const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const port = 5000;

// Middleware to parse JSON data
app.use(bodyParser.json());

// Route to handle uplink data
app.post('/uplink', (req, res) => {
    const data = req.body;
    console.log("Received Uplink:", data);
    // Process the uplink data as needed
    res.status(200).json({ status: 'success' });
});

// Route to handle join notifications
app.post('/join', (req, res) => {
    const data = req.body;
    console.log("Received Join Notification:");
    res.status(200).json({ status: 'success' });
});

// Route to handle ACK notifications
app.post('/ack', (req, res) => {
    const data = req.body;
    console.log("Received ACK:");
    res.status(200).json({ status: 'success' });
});

// Route to handle error notifications
app.post('/error', (req, res) => {
    const data = req.body;
    console.log("Received Error:");
    res.status(200).json({ status: 'success' });
});

// Route to handle downlink notifications
app.post('/downlink', (req, res) => {
    const data = req.body;
    console.log("Received Downlink:");
    res.status(200).json({ status: 'success' });
});

// Start the server
app.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
