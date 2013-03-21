var express = require('express'),
commander = require('./routes/commander');
 
var app = express();

app.get('/login/:username/:password' , commander.login );
app.get('/load/:url/:name' , commander.load ); 
app.get('/play/:name' , commander.play );
app.get('/pause/:name' , commander.pause );
app.get('/search/artist/:query' , commander.searchArtist );


app.listen(3000);
console.log('Listening on port 3000...');
