var request = require('request');
var colors = require('colors');
var config = require('./config.js');

colors.setTheme({ info:'brightMagenta', warning:'brightYellow', error:'brightRed', success:'brightGreen'});


console.log(`Client is trying to connect to server http://${config.server}:${config.port}`.info);

var tryConnectTracker;
tryConnect();

function tryConnect(){
	tryConnectTracker = setTimeout(function() {

		fetchServerStatus(tryConnectCallback);
			
	}, config.interval);
}

function tryConnectCallback(err, data){
	if(data && data.live === true){
		console.log(`[Success] connection established with server`.success);

		if(tryConnectTracker) clearTimeout(tryConnectTracker);
	}
	else{
		console.log(`[Failed] connecting server, trying in ${config.interval/1000} seconds...`.error);

		tryConnect();
	}
}


function fetchServerStatus(callback){

	var liveUrl = `http://${config.server}:${config.port}/${config.api.live}`;

	request.get({ 
		url : liveUrl,
		json : true,
		rejectUnauthorized : false
	}, function(error, response, body){

		if(response && response.statusCode >= 200 && response.statusCode < 300){
			callback(null, body);
		}
		else{
			callback(error, null);
		}

	});
}