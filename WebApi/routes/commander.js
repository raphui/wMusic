var net = require("net");
var http = require("http");

var host = '127.0.0.1'
var port = '1338';

var socket = new net.Socket();
socket.connect( port , host , function() {
	console.log("Connected to: " + host + ":" + port );
});

exports.login = function( req , res ) {
	var code = "";

	socket.write("ACCOUNT#LOGIN#" + req.params.username + "%" + req.params.password );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;

		if( code != "" || code.indexOf("\u0000") != -1 )
			res.send({action:'login' , status: code});
		else
			code = "";
	});

};

exports.logout = function( req , res ) {
	var code = "";
	socket.write("ACCOUNT#LOGOUT");

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'logout' , status: code});
	});
};

exports.load = function( req , res ) {
	var code = "";
	socket.write("STREAMER#LOAD#" + req.params.url + "%" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'load' , status: code});
	});
};

exports.play = function( req , res ) {
	var code = "";
	socket.write("STREAMER#PLAY#" + req.params.name );
	
	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'play' , status: code});
	});
};

exports.pause = function( req , res ) {
	var code = "";
	socket.write("STREAMER#PAUSE#" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'pause' , status: code});
	});
};

exports.next = function( req , res ) {
	var code = "";
	socket.write("STREAMER#NEXT#" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'next' , status: code});
	});
};

exports.infos = function( req , res ) {
	var code = "";
	socket.write("STREAMER#INFOS#" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
		code += data;
		res.send({action:'infos' , status: code});
	});
};

exports.playlistName = function( req , res ) {
	var code = "";
	socket.write("PLAYLIST#LIST");

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
		code += data;
		res.send({action:'playlistName' , status: code});
	});
};

exports.loadPlaylist = function( req , res ) {
	var code = "";
	socket.write("STREAMER#PLLOAD#" + req.params.plid + "%" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'loadPlaylist' , status: code});
	});
};

exports.playlistAdd = function( req , res ) {
	var code = "";
	socket.write("PLAYLIST#ADD#" + req.params.name );

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'playlistAdd' , status: code});
	});
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


exports.listStream = function( req , res ) {
	var code = "";
	socket.write("STREAMER#GETLIST");

	socket.on('data' , function( data ) {
		console.log( data.toString("utf8") );
	    code += data;
		res.send({action:'list' , status: code});
	});

};
