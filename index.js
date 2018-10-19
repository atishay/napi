const express = require('express');
const editor = require('./build/Release/Edit.node');
var bodyParser = require('body-parser');
const app = express();

app.get('/', (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

app.post('/', bodyParser.text({ limit: '50mb' }), (req, res) => {
  const requestBuffer = Buffer.from(req.body.split(",")[1], 'base64');
  editor.edit(requestBuffer, (err, buffer) => {
    res.send("data:image/jpeg;base64," + buffer.toString('base64'));
  })
});

app.listen(3000);
