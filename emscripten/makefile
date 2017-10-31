EMSCRIPTEN_VERSION=1.37.22
EMSCRIPTEN_BITS=64bit
EMDOCKER_IMAGE=trzeci/emscripten:sdk-tag-$(EMSCRIPTEN_VERSION)-$(EMSCRIPTEN_BITS)

.PHONY: all

OPTIMIZE+=-O2

CFLAGS +=  -Wall -Werror -fno-strict-aliasing $(OPTIMIZE)
CXXFLAGS += -Wall -Werror --std=c++11 $(OPTIMIZE)

CFLAGS += -msse2

EXPORTED_FUNCTIONS=-s EXPORTED_FUNCTIONS='["_keccak1600", "_keccakf", "_keccak"]'
EMSCRIPTEN_SETTINGS=-s "BINARYEN_METHOD='native-wasm'" -s WASM=1 -s  DISABLE_EXCEPTION_CATCHING=1 \
				     -s NO_FILESYSTEM=1 $(EXPORTED_FUNCTIONS) \
				      --post-js emscripten.keccak.post.js
EM_CC_COMPILER  = docker run -v $(CURDIR):/src $(EMDOCKER_IMAGE) emcc $(EMSCRIPTEN_SETTINGS)
EM_CXX_COMPILER = docker run -v $(CURDIR):/src $(EMDOCKER_IMAGE) em++ $(EMSCRIPTEN_SETTINGS)


all: clean build/emscripten/keccak.js



build/emscripten/%.js: %.c
	$(EM_CC_COMPILER) $(CFLAGS) -o $@ $<

clean:
	rm -f build/emscripten/*