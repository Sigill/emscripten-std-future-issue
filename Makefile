EMPP ?= em++

all: future.js

clean:
	rm -rf future.js future.wasm future.worker.js

.PHONY : all clean

future.js: future.cxx
	$(EMPP) $< -std=c++14 -s ALLOW_MEMORY_GROWTH=1 -s USE_PTHREADS=1 -O3 -sPTHREAD_POOL_SIZE=20 --bind -o $@
