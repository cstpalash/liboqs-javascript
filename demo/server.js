const express = require('express');
const app = express();
var colors = require('colors');
var config = require('./config.js');
var path = require('path');

colors.setTheme({ info:'brightMagenta', warning:'brightYellow', error:'brightRed', success:'brightGreen'});


app.get(`/${config.api.live}`, live); //live api

app.get(`/${config.api.publicKey}`, getPublicKey); //live api

app.listen(config.port, () => console.log(`Demo server is listening at http://${config.server}:${config.port}`.info));


function live(req, res){
	return res.json({ live : true });
}

function getPublicKey(req, res){
	return res.sendFile(path.join(__dirname, '../public-key'));
}