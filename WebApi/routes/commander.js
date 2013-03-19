exports.play = function( req , res ) {
	res.send([{action:'play'}]);
};

exports.pause = function( req , res ) {
	res.send([{action:'pause'}]);
};
