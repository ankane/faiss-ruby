#include <rice/rice.hpp>

#include "numo.hpp"
#include "utils.h"

size_t check_shape(const numo::NArray& objects, size_t k) {
  auto ndim = objects.ndim();
  if (ndim != 2) {
    throw Rice::Exception(rb_eArgError, "expected 2 dimensions, not %d", ndim);
  }
  auto shape = objects.shape();
  if (shape[1] != k) {
    throw Rice::Exception(rb_eArgError, "expected 2nd dimension to be %d, not %d", k, shape[1]);
  }
  return shape[0];
}

void check_frozen(const Rice::Object& obj) {
  if (obj.is_frozen()) {
    // same message as rb_check_frozen
    // but must use C++ exception to avoid skipping destructors and leaking memory
    throw Rice::Exception(rb_eFrozenError, "can't modify frozen %" PRIsVALUE ": ", CLASS_OF(obj.value()));
  }
}
