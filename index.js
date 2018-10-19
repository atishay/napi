const express = require('express');
const editor = require('./build/Release/Edit.node');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
var http = require('http');
var https = require('https');
const privateKey = fs.readFileSync('cert.key', 'utf8');
const certificate = fs.readFileSync('cert.pem', 'utf8');
var credentials = { key: privateKey, cert: certificate };

app.get('/', (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

app.post('/', bodyParser.text({ limit: '50mb' }), (req, res) => {
  const requestBuffer = Buffer.from(req.body.split(",")[1], 'base64');
  editor.edit(requestBuffer, (err, buffer) => {
    res.send("data:image/jpeg;base64," + buffer.toString('base64'));
  });
});

let port = process.env.PORT;
if (port == null || port == "") {
  port = 8080;
}
const httpServer = http.createServer(app);
const httpsServer = https.createServer(credentials, app);
httpServer.listen(port);
httpsServer.listen(8443);

