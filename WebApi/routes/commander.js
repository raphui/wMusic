var net = require("net");
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
