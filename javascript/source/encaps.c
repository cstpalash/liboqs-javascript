#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oqs/oqs.h>

#include <emscripten.h>
#include "util.h"
#include "constant.h"


void cleanup_heap(uint8_t *secret_key, uint8_t *shared_secret_e,
                  uint8_t *shared_secret_d, uint8_t *public_key,
                  uint8_t *ciphertext, OQS_KEM *kem);


EMSCRIPTEN_KEEPALIVE
int Encaps(const char *algo){

	char publicKeyFile[] = PUBLIC_KEY_FILE_NAME;
	char aesKeyFile[] = AES_KEY_CLIENT_FILE_NAME;
	char cipherTextFile[] = CIPHER_TEXT_FILE_NAME;

	printf("Algo: %s\n", algo);

	OQS_KEM *kem = NULL;
	uint8_t *public_key = NULL;
	uint8_t *secret_key = NULL;
	uint8_t *ciphertext = NULL;
	uint8_t *shared_secret_e = NULL;
	uint8_t *shared_secret_d = NULL;

	kem = OQS_KEM_new(algo);
	if (kem == NULL) {
		fprintf(stderr, "ERROR: not able to initialize with given algo!\n");
		return 1;
	}

	public_key = malloc(kem->length_public_key);
	secret_key = malloc(kem->length_secret_key);
	ciphertext = malloc(kem->length_ciphertext);
	shared_secret_e = malloc(kem->length_shared_secret);
	shared_secret_d = malloc(kem->length_shared_secret);

	if ((public_key == NULL) || (secret_key == NULL) || (ciphertext == NULL) || (shared_secret_e == NULL) || (shared_secret_d == NULL)) {
		fprintf(stderr, "ERROR: malloc failed\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);
		return 1;
	}

	if(!readFile(public_key, kem->length_public_key, publicKeyFile)){
		fprintf(stderr, "ERROR: public key file read failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return 1;
	}

	OQS_STATUS rc = OQS_KEM_encaps(kem, ciphertext, shared_secret_e, public_key);
	if (rc != OQS_SUCCESS) {
		fprintf(stderr, "ERROR: OQS_KEM_encaps failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return 1;
	}

	if(!writeFile(shared_secret_e, kem->length_shared_secret, aesKeyFile)){
		fprintf(stderr, "ERROR: AES key file save failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return 1;
	}

	if(!writeFile(ciphertext, kem->length_ciphertext, cipherTextFile)){
		fprintf(stderr, "ERROR: Cipher text file save failed!\n");
		cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

		return 1;
	}

	printf("Success: encryption.\n");

	printf("[Success] aes_key_length: %lu and cipher_text_length: %lu \n", kem->length_shared_secret, kem->length_ciphertext);

	cleanup_heap(secret_key, shared_secret_e, shared_secret_d, public_key,
		             ciphertext, kem);

	return 0;
}


void cleanup_heap(uint8_t *secret_key, uint8_t *shared_secret_e,
                  uint8_t *shared_secret_d, uint8_t *public_key,
                  uint8_t *ciphertext, OQS_KEM *kem) {
	if (kem != NULL) {
		OQS_MEM_secure_free(secret_key, kem->length_secret_key);
		OQS_MEM_secure_free(shared_secret_e, kem->length_shared_secret);
		OQS_MEM_secure_free(shared_secret_d, kem->length_shared_secret);
	}
	OQS_MEM_insecure_free(public_key);
	OQS_MEM_insecure_free(ciphertext);
	OQS_KEM_free(kem);
}