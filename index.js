/**
 * Basic Express.js app to serve the demo for N-API
 */
const express = require('express');
const editor = require('./build/Release/Edit.node');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
var http = require('http');
var https = require('https');

// Keys for SSL
const privateKey = fs.readFileSync('key.pem', 'utf8');
const certificate = fs.readFileSync('cert.pem', 'utf8');
var credentials = { key: privateKey, cert: certificate };

// Provide index.html on the basic request
app.get('/', (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

// The only API - post to perform the image editing.
app.post('/', bodyParser.text({ limit: '50mb' }), (req, res) => {
  const requestBuffer = Buffer.from(req.body.split(",")[1], 'base64');
  editor.edit(requestBuffer, (err, buffer) => {
    res.send("data:image/jpeg;base64," + buffer.toString('base64'));
  });
});

// Launch the http and http servers.
const httpServer = http.createServer(app);
const httpsServer = https.createServer(credentials, app);
httpServer.listen(8080);
httpsServer.listen(8443);

