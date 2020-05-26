# liboqs-javascript

This is a javascript wrapper on [liboqs C library](https://github.com/open-quantum-safe/liboqs)

## How to test

```
clone this repo into your local
cd <cloned repo>
npm install
npm text
```

## How to build (optional)

Download and setup [emscripten](https://emscripten.org/docs/getting_started/downloads.html#installation-instructions). We shall use emcc to build this repo.
C files (source) can be found under javascript/source folder, emcc will generate new js/wasm files under javascript folder.

```
clone this repo into your local
cd <cloned repo>
npm install

//build keygen
emcc -Ibuild/include -Wall -s TOTAL_MEMORY=22216704 -s NODERAWFS=1 -Lbuild/lib javascript/source/keygen.c build/lib/liboqs.a -o javascript/keygen.js -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s ALLOW_MEMORY_GROWTH=1

//build encaps
emcc -Ibuild/include -Wall -s TOTAL_MEMORY=22216704 -s NODERAWFS=1 -Lbuild/lib javascript/source/encaps.c build/lib/liboqs.a -o javascript/encaps.js -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s ALLOW_MEMORY_GROWTH=1

//build encaps
emcc -Ibuild/include -Wall -s TOTAL_MEMORY=22216704 -s NODERAWFS=1 -Lbuild/lib javascript/source/decaps.c build/lib/liboqs.a -o javascript/decaps.js -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s ALLOW_MEMORY_GROWTH=1
```
