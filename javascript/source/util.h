#include <stdio.h>

bool writeFile(const uint8_t *mem, size_t len, const char *fname){
	FILE *fp = fopen(fname, "wb");

	if(fp == NULL){
		perror(fname);
		return false;
	}

	if(fwrite(mem, len, 1, fp) != 1){
		perror(fname);
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;

}

bool readFile(uint8_t *mem, size_t len, const char *fname){
	FILE *fp = fopen(fname, "rb");

	if(fp == NULL){
		perror(fname);
		return false;
	}

	if(fread(mem, len, 1, fp) != 1){
		if(ferror(fp))
			perror(fname);
		else
			fprintf(stderr, "Error: %s shorter than %zd\n", fname, len);

		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;

}