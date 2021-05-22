#include "utils.h"

VALUE numo::NArray::value() const {
  return this->_value;
}

size_t numo::NArray::ndim() {
  return RNARRAY_NDIM(this->_value);
}

size_t* numo::NArray::shape() {
  return RNARRAY_SHAPE(this->_value);
}

// need a contiguous array for pointers
void numo::NArray::construct_value(VALUE dtype, VALUE v) {
  v = rb_funcall(dtype, rb_intern("cast"), 1, v);
  if (nary_check_contiguous(v) == Qfalse) {
    v = nary_dup(v);
  }
  this->_value = v;
}

void numo::NArray::construct_list(VALUE dtype, std::initializer_list<size_t> s) {
  auto shape = ALLOCA_N(size_t, s.size());
  size_t i = 0;
  for (auto& v : s) {
    shape[i++] = v;
  }
  this->_value = rb_narray_new(dtype, s.size(), shape);
}

const float* numo::SFloat::read_ptr() {
  return reinterpret_cast<float*>(nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value));
}

float* numo::SFloat::write_ptr() {
  return reinterpret_cast<float*>(nary_get_pointer_for_write(this->_value));
}

const uint8_t* numo::UInt8::read_ptr() {
  return reinterpret_cast<uint8_t*>(nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value));
}

uint8_t* numo::UInt8::write_ptr() {
  return reinterpret_cast<uint8_t*>(nary_get_pointer_for_write(this->_value));
}

const int32_t* numo::Int32::read_ptr() {
  return reinterpret_cast<int32_t*>(nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value));
}

int32_t* numo::Int32::write_ptr() {
  return reinterpret_cast<int32_t*>(nary_get_pointer_for_write(this->_value));
}

const int64_t* numo::Int64::read_ptr() {
  return reinterpret_cast<int64_t*>(nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value));
}

int64_t* numo::Int64::write_ptr() {
  return reinterpret_cast<int64_t*>(nary_get_pointer_for_write(this->_value));
}

size_t check_shape(numo::NArray objects, size_t k) {
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
