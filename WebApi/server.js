var express = require('express'),
commander = require('./routes/commander');
 
var app = express();

app.get('/login/:username/:password' , commander.login );
app.get('/logout' , commander.logout );
app.get('/load/:url/:name' , commander.load ); 
app.get('/play/:name' , commander.play );
app.get('/pause/:name' , commander.pause );
app.get('/next/:name' , commander.next );
app.get('/infos/:name' , commander.infos );
app.get('/playlist/list' , commander.playlistName );
app.get('/playlist/add/:name' , commander.playlistAdd );
app.get('/playlist/load/:plid/:name' , commander.loadPlaylist );
app.get('/search/artist/:query' , commander.searchArtist );
app.get('/stream/list' , commander.listStream );
app.get('/stream/volume/:vol' , commander.setVolume );

app.listen(3000);
console.log('Listening on port 3000...');
