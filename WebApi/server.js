var express = require('express'),
commander = require('./routes/commander');
 
var app = express();

app.get('/load/:url/:name' , commander.load ); 
app.get('/play/:name' , commander.play );
app.get('/pause/:name' , commander.pause );
 
app.listen(3000);
console.log('Listening on port 3000...');
