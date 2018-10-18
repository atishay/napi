const express = require('express');
const editor = require('./build/Release/Edit.node')
const app = express();

app.get('/', (req, res) => {
  res.sendFile(__dirname + "/index.html");
});

app.post('/', (req, res) => {
  editor.edit(Buffer.from(req.body, 'base64'), (buffer) => {
    res.send("data:image/jpeg;base64," + buffer.toString('base64'));
  })
});

app.listen(3000);
