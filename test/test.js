var assert = require('assert');
var expect  = require('chai').expect;
var file = require('chai-files').file;

var algo = 'FrodoKEM-640-AES';

describe("liboqs", function(){
	this.timeout(10000);

	it("keygen should generate public and secret key", function(done){

		var liboqs_keygen = require("../javascript/keygen.js");
		liboqs_keygen["onRuntimeInitialized"] = function(){
			var status = liboqs_keygen.ccall('GetKeyPair', 'number', ['string'], [ algo ]);

			expect(status).to.equal(0);

			expect(file('public-key')).to.exist;
			expect(file('secret-key')).to.exist;

			expect(file('public-key')).to.not.be.empty;
			expect(file('secret-key')).to.not.be.empty;

			done();
		}

	});

	it("encaps should use public key to generate cipher text and aes key", function(done){

		var liboqs_encaps = require("../javascript/encaps.js");
		liboqs_encaps["onRuntimeInitialized"] = function(){
			var status = liboqs_encaps.ccall('Encaps', 'number', ['string'], [ algo ]);

			expect(status).to.equal(0);

			expect(file('cipher-text')).to.exist;
			expect(file('aes-key-client')).to.exist;

			expect(file('cipher-text')).to.not.be.empty;
			expect(file('aes-key-client')).to.not.be.empty;

			done();
		}

	});

	it("decaps should use secret key and cipher text to generate aes key", function(done){

		var liboqs_decaps = require("../javascript/decaps.js");
		liboqs_decaps["onRuntimeInitialized"] = function(){
			var status = liboqs_decaps.ccall('Decaps', 'number', ['string'], [ algo ]);

			expect(status).to.equal(0);

			expect(file('aes-key-server')).to.exist;

			expect(file('aes-key-server')).to.not.be.empty;

			done();
		}

	});
	
	
	it("encaps and decaps should generate same aes key files", function(done){

		expect(file('aes-key-client').content).to.equal(file('aes-key-server').content);

		done();

	});
	
	
});
