var express = require('express'),
commander = require('./routes/commander');
 
var app = express();
 
app.get('/play', commander.play);
app.get('/pause', commander.pause);
 
app.listen(3000);
console.log('Listening on port 3000...');
