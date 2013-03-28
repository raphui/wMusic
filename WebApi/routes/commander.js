var net = require("net");
var http = require("http");

var host = '127.0.0.1'
var port = '1338';

var socket = new net.Socket();
socket.connect( port , host , function() {
	console.log("Connected to: " + host + ":" + port );
});

exports.login = function( req , res ) {
	res.send({action:'login' , status:'OK'});
	socket.write("ACCOUNT#LOGIN#" + req.params.username + "%" + req.params.password );
};

exports.load = function( req , res ) {
	res.send({action:'load' , status:'OK'});
	socket.write("STREAMER#LOAD#" + req.params.url + "%" + req.params.name );
};

exports.play = function( req , res ) {
	res.send({action:'play' , status:'OK'});
	socket.write("STREAMER#PLAY#" + req.params.name );
};

exports.pause = function( req , res ) {
	res.send({action:'pause' , status:'OK'});
	socket.write("STREAMER#PAUSE#" + req.params.name );
};

exports.next = function( req , res ) {
	res.send({action:'next' , status:'OK'});
	socket.write("STREAMER#NEXT#" + req.params.name );
};

exports.infos = function( req , res ) {
	res.send({action:'infos' , status:'OK'});
	socket.write("STREAMER#INFOS#" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    res.send( data );
	});
};

exports.playlistName = function( req , res ) {
	//res.send({action:'playlistName' , status:'OK'});
	socket.write("PLAYLIST#LIST");

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    res.send( data );
	});
};

exports.loadPlaylist = function( req , res ) {
	res.send({action:'loadPlaylist' , status:'OK'});
	socket.write("STREAMER#PLLOAD#" + req.params.plid + "%" + req.params.name );
};

exports.playlistAdd = function( req , res ) {
	res.send({action:'playlistAdd' , status:'OK'});
	socket.write("PLAYLIST#ADD#" + req.params.name );
};

exports.searchArtist = function( req , res ) {

	var query = '/search/1/artist.json?q=' + req.params.query;

	var httpOptions = {

		host: 'ws.spotify.com',
		port: '80',
		path: query,
		method: 'GET',
		headers: {
			'Content-Type': 'application/json'
		}

	};

	http.get(httpOptions , function( response ) {
		response.setEncoding('utf8');
	  	response.on("data" , function( chunk ) {
	    console.log("BODY: " + chunk );
	    res.send( chunk );
	  });

	}).on('error' , function( e ) {	
	  console.log("Got error: " + e.message );
	});


};
