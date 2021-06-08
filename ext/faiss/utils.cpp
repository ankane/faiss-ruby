#include "utils.h"

size_t check_shape(numo::NArray& objects, size_t k) {
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
