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

const float* numo::SFloat::read_ptr() {
  return (float*) nary_get_pointer_for_read(this->_value);
}

float* numo::SFloat::write_ptr() {
  return (float*) nary_get_pointer_for_write(this->_value);
}

const uint8_t* numo::UInt8::read_ptr() {
  return (uint8_t*) nary_get_pointer_for_read(this->_value);
}

uint8_t* numo::UInt8::write_ptr() {
  return (uint8_t*) nary_get_pointer_for_write(this->_value);
}

const int32_t* numo::Int32::read_ptr() {
  return (int32_t*) nary_get_pointer_for_read(this->_value);
}

int32_t* numo::Int32::write_ptr() {
  return (int32_t*) nary_get_pointer_for_write(this->_value);
}

const int64_t* numo::Int64::read_ptr() {
  return (int64_t*) nary_get_pointer_for_read(this->_value);
}

int64_t* numo::Int64::write_ptr() {
  return (int64_t*) nary_get_pointer_for_write(this->_value);
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
