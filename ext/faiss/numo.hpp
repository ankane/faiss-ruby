/*!
 * Numo.hpp v0.1.0
 * https://github.com/ankane/numo.hpp
 * BSD-2-Clause License
 */

#pragma once

#include <rice/rice.hpp>
#include <numo/narray.h>

namespace numo {
  class NArray {
  public:
    NArray(VALUE v) {
      construct_value(this->dtype(), v);
    }

    NArray(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    VALUE value() const {
      return this->_value;
    }

    size_t ndim() {
      return RNARRAY_NDIM(this->_value);
    }

    size_t* shape() {
      return RNARRAY_SHAPE(this->_value);
    }

    size_t size() {
      return RNARRAY_SIZE(this->_value);
    }

    bool is_contiguous() {
      return nary_check_contiguous(this->_value) == Qtrue;
    }

    operator Rice::Object() const {
      return Rice::Object(this->_value);
    }

    const void* read_ptr() {
      if (!is_contiguous()) {
        this->_value = nary_dup(this->_value);
      }
      return nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value);
    }

    void* write_ptr() {
      return nary_get_pointer_for_write(this->_value);
    }

  protected:
    NArray() { }

    void construct_value(VALUE dtype, VALUE v) {
      this->_value = rb_funcall(dtype, rb_intern("cast"), 1, v);
    }

    void construct_shape(VALUE dtype, std::initializer_list<size_t> shape) {
      // rb_narray_new doesn't modify shape, but not marked as const
      this->_value = rb_narray_new(dtype, shape.size(), const_cast<size_t*>(shape.begin()));
    }

    VALUE _value;

  private:
    VALUE dtype() {
      return numo_cNArray;
    }
  };

  class SFloat: public NArray {
  public:
    SFloat(VALUE v) {
      construct_value(this->dtype(), v);
    }

    SFloat(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    SFloat(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const float* read_ptr() {
      return reinterpret_cast<const float*>(NArray::read_ptr());
    }

    float* write_ptr() {
      return reinterpret_cast<float*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cSFloat;
    }
  };

  class DFloat: public NArray {
  public:
    DFloat(VALUE v) {
      construct_value(this->dtype(), v);
    }

    DFloat(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    DFloat(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const double* read_ptr() {
      return reinterpret_cast<const double*>(NArray::read_ptr());
    }

    double* write_ptr() {
      return reinterpret_cast<double*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cDFloat;
    }
  };

  class Int8: public NArray {
  public:
    Int8(VALUE v) {
      construct_value(this->dtype(), v);
    }

    Int8(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    Int8(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const int8_t* read_ptr() {
      return reinterpret_cast<const int8_t*>(NArray::read_ptr());
    }

    int8_t* write_ptr() {
      return reinterpret_cast<int8_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cInt8;
    }
  };

  class Int16: public NArray {
  public:
    Int16(VALUE v) {
      construct_value(this->dtype(), v);
    }

    Int16(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    Int16(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const int16_t* read_ptr() {
      return reinterpret_cast<const int16_t*>(NArray::read_ptr());
    }

    int16_t* write_ptr() {
      return reinterpret_cast<int16_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cInt16;
    }
  };

  class Int32: public NArray {
  public:
    Int32(VALUE v) {
      construct_value(this->dtype(), v);
    }

    Int32(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    Int32(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const int32_t* read_ptr() {
      return reinterpret_cast<const int32_t*>(NArray::read_ptr());
    }

    int32_t* write_ptr() {
      return reinterpret_cast<int32_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cInt32;
    }
  };

  class Int64: public NArray {
  public:
    Int64(VALUE v) {
      construct_value(this->dtype(), v);
    }

    Int64(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    Int64(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const int64_t* read_ptr() {
      return reinterpret_cast<const int64_t*>(NArray::read_ptr());
    }

    int64_t* write_ptr() {
      return reinterpret_cast<int64_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cInt64;
    }
  };

  class UInt8: public NArray {
  public:
    UInt8(VALUE v) {
      construct_value(this->dtype(), v);
    }

    UInt8(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    UInt8(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const uint8_t* read_ptr() {
      return reinterpret_cast<const uint8_t*>(NArray::read_ptr());
    }

    uint8_t* write_ptr() {
      return reinterpret_cast<uint8_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cUInt8;
    }
  };

  class UInt16: public NArray {
  public:
    UInt16(VALUE v) {
      construct_value(this->dtype(), v);
    }

    UInt16(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    UInt16(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const uint16_t* read_ptr() {
      return reinterpret_cast<const uint16_t*>(NArray::read_ptr());
    }

    uint16_t* write_ptr() {
      return reinterpret_cast<uint16_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cUInt16;
    }
  };

  class UInt32: public NArray {
  public:
    UInt32(VALUE v) {
      construct_value(this->dtype(), v);
    }

    UInt32(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    UInt32(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const uint32_t* read_ptr() {
      return reinterpret_cast<const uint32_t*>(NArray::read_ptr());
    }

    uint32_t* write_ptr() {
      return reinterpret_cast<uint32_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cUInt32;
    }
  };

  class UInt64: public NArray {
  public:
    UInt64(VALUE v) {
      construct_value(this->dtype(), v);
    }

    UInt64(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    UInt64(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const uint64_t* read_ptr() {
      return reinterpret_cast<const uint64_t*>(NArray::read_ptr());
    }

    uint64_t* write_ptr() {
      return reinterpret_cast<uint64_t*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cUInt64;
    }
  };

  class SComplex: public NArray {
  public:
    SComplex(VALUE v) {
      construct_value(this->dtype(), v);
    }

    SComplex(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    SComplex(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

  private:
    VALUE dtype() {
      return numo_cSComplex;
    }
  };

  class DComplex: public NArray {
  public:
    DComplex(VALUE v) {
      construct_value(this->dtype(), v);
    }

    DComplex(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    DComplex(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

  private:
    VALUE dtype() {
      return numo_cDComplex;
    }
  };

  class Bit: public NArray {
  public:
    Bit(VALUE v) {
      construct_value(this->dtype(), v);
    }

    Bit(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    Bit(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

  private:
    VALUE dtype() {
      return numo_cBit;
    }
  };

  class RObject: public NArray {
  public:
    RObject(VALUE v) {
      construct_value(this->dtype(), v);
    }

    RObject(Rice::Object o) {
      construct_value(this->dtype(), o.value());
    }

    RObject(std::initializer_list<size_t> shape) {
      construct_shape(this->dtype(), shape);
    }

    const VALUE* read_ptr() {
      return reinterpret_cast<const VALUE*>(NArray::read_ptr());
    }

    VALUE* write_ptr() {
      return reinterpret_cast<VALUE*>(NArray::write_ptr());
    }

  private:
    VALUE dtype() {
      return numo_cRObject;
    }
  };
}

namespace Rice::detail {
  template<>
  struct Type<numo::NArray>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::NArray>
  {
  public:
    numo::NArray convert(VALUE x)
    {
      return numo::NArray(x);
    }
  };

  template<>
  class To_Ruby<numo::NArray>
  {
  public:
    VALUE convert(const numo::NArray& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::SFloat>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::SFloat>
  {
  public:
    numo::SFloat convert(VALUE x)
    {
      return numo::SFloat(x);
    }
  };

  template<>
  class To_Ruby<numo::SFloat>
  {
  public:
    VALUE convert(const numo::SFloat& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::DFloat>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::DFloat>
  {
  public:
    numo::DFloat convert(VALUE x)
    {
      return numo::DFloat(x);
    }
  };

  template<>
  class To_Ruby<numo::DFloat>
  {
  public:
    VALUE convert(const numo::DFloat& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::Int8>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::Int8>
  {
  public:
    numo::Int8 convert(VALUE x)
    {
      return numo::Int8(x);
    }
  };

  template<>
  class To_Ruby<numo::Int8>
  {
  public:
    VALUE convert(const numo::Int8& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::Int16>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::Int16>
  {
  public:
    numo::Int16 convert(VALUE x)
    {
      return numo::Int16(x);
    }
  };

  template<>
  class To_Ruby<numo::Int16>
  {
  public:
    VALUE convert(const numo::Int16& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::Int32>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::Int32>
  {
  public:
    numo::Int32 convert(VALUE x)
    {
      return numo::Int32(x);
    }
  };

  template<>
  class To_Ruby<numo::Int32>
  {
  public:
    VALUE convert(const numo::Int32& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::Int64>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::Int64>
  {
  public:
    numo::Int64 convert(VALUE x)
    {
      return numo::Int64(x);
    }
  };

  template<>
  class To_Ruby<numo::Int64>
  {
  public:
    VALUE convert(const numo::Int64& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::UInt8>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::UInt8>
  {
  public:
    numo::UInt8 convert(VALUE x)
    {
      return numo::UInt8(x);
    }
  };

  template<>
  class To_Ruby<numo::UInt8>
  {
  public:
    VALUE convert(const numo::UInt8& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::UInt16>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::UInt16>
  {
  public:
    numo::UInt16 convert(VALUE x)
    {
      return numo::UInt16(x);
    }
  };

  template<>
  class To_Ruby<numo::UInt16>
  {
  public:
    VALUE convert(const numo::UInt16& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::UInt32>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::UInt32>
  {
  public:
    numo::UInt32 convert(VALUE x)
    {
      return numo::UInt32(x);
    }
  };

  template<>
  class To_Ruby<numo::UInt32>
  {
  public:
    VALUE convert(const numo::UInt32& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::UInt64>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::UInt64>
  {
  public:
    numo::UInt64 convert(VALUE x)
    {
      return numo::UInt64(x);
    }
  };

  template<>
  class To_Ruby<numo::UInt64>
  {
  public:
    VALUE convert(const numo::UInt64& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::SComplex>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::SComplex>
  {
  public:
    numo::SComplex convert(VALUE x)
    {
      return numo::SComplex(x);
    }
  };

  template<>
  class To_Ruby<numo::SComplex>
  {
  public:
    VALUE convert(const numo::SComplex& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::DComplex>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::DComplex>
  {
  public:
    numo::DComplex convert(VALUE x)
    {
      return numo::DComplex(x);
    }
  };

  template<>
  class To_Ruby<numo::DComplex>
  {
  public:
    VALUE convert(const numo::DComplex& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::Bit>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::Bit>
  {
  public:
    numo::Bit convert(VALUE x)
    {
      return numo::Bit(x);
    }
  };

  template<>
  class To_Ruby<numo::Bit>
  {
  public:
    VALUE convert(const numo::Bit& x) {
      return x.value();
    }
  };

  template<>
  struct Type<numo::RObject>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<numo::RObject>
  {
  public:
    numo::RObject convert(VALUE x)
    {
      return numo::RObject(x);
    }
  };

  template<>
  class To_Ruby<numo::RObject>
  {
  public:
    VALUE convert(const numo::RObject& x) {
      return x.value();
    }
  };
}
