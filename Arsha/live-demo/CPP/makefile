
EMSCRIPTEN_DIR = /Users/nicholaslarsen/Documents/7349_Projects/emsdk/upstream/emscripten
EMSCRIPTEN_ROOT = /Users/nicholaslarsen/Documents/7349_Projects/emsdk/
SOURCE_DIR = /Users/nicholaslarsen/Documents/7349_Projects/Convolution-Product-Website/Arsha/live-demo/CPP
OBJECTS_DIR = /Users/nicholaslarsen/Documents/7349_Projects/Convolution-Product-Website/Arsha/live-demo


set_node:
	cd $(EMSCRIPTEN_ROOT) ;\
	source emsdk_env.sh ;\
	source ~/.bash_profile ;\
	nvm use 19.0

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

glue: set_node
	cd $(EMSCRIPTEN_DIR);\
	python3 tools/webidl_binder.py $(SOURCE_DIR)/conv_filters.idl $(SOURCE_DIR)/glue

build: set_node
	cd $(EMSCRIPTEN_DIR);\
	./em++ $(SOURCE_DIR)/convolutionFilters.cpp $(SOURCE_DIR)/main_fake.cpp \
	$(SOURCE_DIR)/my_glue_wrapper.cpp \
    -sINITIAL_MEMORY=1073741824 \
    -std="c++11" \
    -s USE_PTHREADS -s  -sPTHREAD_POOL_SIZE=16\
    --post-js /$(SOURCE_DIR)/glue.js -o \
    $(OBJECTS_DIR)/output_test.js -O2 -s \
    EXPORTED_FUNCTIONS="['_malloc', '_free']" 

#-s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=2

clean:
	$(SOURCE_DIR)/glue.js $(SOURCE_DIR)/glue.cpp $(SOURCE_DIR)/output.wasm 