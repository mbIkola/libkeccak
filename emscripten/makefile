EMSCRIPTEN_VERSION=1.37.22
EMSCRIPTEN_BITS=64bit
EMDOCKER_IMAGE=trzeci/emscripten:sdk-tag-$(EMSCRIPTEN_VERSION)-$(EMSCRIPTEN_BITS)

.PHONY: all

OPTIMIZE+=-O2

CFLAGS +=  -Wall -Werror -Iinclude -fno-strict-aliasing $(OPTIMIZE)
CXXFLAGS += -Wall -Werror --std=c++11 $(OPTIMIZE)

CFLAGS += -msse2


EMSCRIPTEN_SETTINGS=-s "BINARYEN_METHOD='native-wasm'" -s WASM=1 -s  DISABLE_EXCEPTION_CATCHING=1 \
				     -s NO_FILESYSTEM=1  \
				     -s WARN_UNALIGNED=1  -s NO_EXIT_RUNTIME=1  \
				     -s ERROR_ON_MISSING_LIBRARIES=1 \
				     -s USE_SDL=0 -s USE_ZLIB=0  -s ASSERTIONS=0
#				      --post-js emscripten/emscripten.keccak.post.js
EM_CC_COMPILER  = docker run -v $(CURDIR)/../:/src $(EMDOCKER_IMAGE) emcc $(EMSCRIPTEN_SETTINGS)
EM_CXX_COMPILER = docker run -v $(CURDIR)/../:/src $(EMDOCKER_IMAGE) em++ $(EMSCRIPTEN_SETTINGS)


all: clean ../dist/emscripten/keccak.js 



../dist/emscripten/keccak.js: 
	@mkdir -p ../dist/emscripten
	$(EM_CC_COMPILER) $(CFLAGS) -o dist/emscripten/keccak.html src/keccak.c



clean:
	rm -f ../dist/emscripten/*
