
EMSCRIPTEN_DIR = /Users/nicholaslarsen/Documents/7349_Projects/emsdk/upstream/emscripten
SOURCE_DIR = /Users/nicholaslarsen/Documents/7349_Projects/lab-2-multithreading-nlarsensmu/Code
compile_opencv:
	make

compile_unit_tests:
	clang++ -O2 -std=c++14 tests.cpp ConvolutionFilters.cpp -o ../Objects/tests
	../Objects/tests

emsdk_source:
	cd /Users/nicholaslarsen/Documents/7349_Projects/emsdk/;\
	source emsdk_env.sh

compile:
	emcc -g foo.cpp main.cpp -o ../Objects/tests_node

glue:
	cd $(EMSCRIPTEN_DIR);\
	python3 tools/webidl_binder.py $(SOURCE_DIR)/conv_filters.idl $(SOURCE_DIR)/glue

build:
	cd $(EMSCRIPTEN_DIR);\
	./emcc $(SOURCE_DIR)/convolutionFilters.cpp $(SOURCE_DIR)/main_fake.cpp \
	$(SOURCE_DIR)/my_glue_wrapper.cpp --post-js \
	$(SOURCE_DIR)/glue.js -o \
	$(SOURCE_DIR)/output.js -O2 -s EXPORTED_FUNCTIONS="['_malloc', '_free']" -s EXPORTED_RUNTIME_METHODS=["ccall, cwrap"] -sLLD_REPORT_UNDEFINED \
	-sALLOW_MEMORY_GROWTH 

clean:
	$(SOURCE_DIR)/glue.js $(SOURCE_DIR)/glue.cpp $(SOURCE_DIR)/output.wasm 