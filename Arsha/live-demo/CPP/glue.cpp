
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.

EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// ConvolutionFilters

ConvolutionFilters* EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters_ConvolutionFilters_0() {
  return new ConvolutionFilters();
}

ConvolutionFilters* EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters_ConvolutionFilters_4(int rows, int cols, int channels, bool debug) {
  return new ConvolutionFilters(rows, cols, channels, debug);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters_applyFilter_1(ConvolutionFilters* self, char* filterName) {
  return self->applyFilter(filterName);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters_getInputImgPtr_0(ConvolutionFilters* self) {
  return self->getInputImgPtr();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters_setThreadCount_1(ConvolutionFilters* self, int num) {
  self->setThreadCount(num);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConvolutionFilters___destroy___0(ConvolutionFilters* self) {
  delete self;
}

}

