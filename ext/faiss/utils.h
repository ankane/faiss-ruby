#pragma once

#include <rice/rice.hpp>
#include <numo/narray.h>

namespace numo {
  class NArray {
  public:
    VALUE value() const {
      return this->_value;
    }

    size_t ndim() {
      return RNARRAY_NDIM(this->_value);
    }

    size_t* shape() {
      return RNARRAY_SHAPE(this->_value);
    }

    const void* read_ptr() {
      return nary_get_pointer_for_read(this->_value) + nary_get_offset(this->_value);
    }

    void* write_ptr() {
      return nary_get_pointer_for_write(this->_value);
    }

  protected:
    void construct_value(VALUE dtype, VALUE v) {
      v = rb_funcall(dtype, rb_intern("cast"), 1, v);
      if (nary_check_contiguous(v) == Qfalse) {
        v = nary_dup(v);
      }
      this->_value = v;
    }

    template<std::size_t N>
    void construct_shape(VALUE dtype, const size_t(&shape)[N]) {
      // rb_narray_new doesn't modify shape, but not marked as const
      this->_value = rb_narray_new(dtype, N, const_cast<size_t*>(shape));
    }

    VALUE _value;
  };

  class SFloat: public NArray {
  public:
    SFloat(VALUE v) {
      construct_value(numo_cSFloat, v);
    }

    template<std::size_t N>
    SFloat(const size_t(&shape)[N]) {
      construct_shape<N>(numo_cSFloat, shape);
    }

    const float* read_ptr() {
      return reinterpret_cast<const float*>(NArray::read_ptr());
    }

    float* write_ptr() {
      return reinterpret_cast<float*>(NArray::write_ptr());
    }
  };
}

namespace numo {
  class UInt8: public NArray {
  public:
    UInt8(VALUE v) {
      construct_value(numo_cUInt8, v);
    }

    template<std::size_t N>
    UInt8(const size_t(&shape)[N]) {
      construct_shape<N>(numo_cUInt8, shape);
    }

    const uint8_t* read_ptr() {
      return reinterpret_cast<const uint8_t*>(NArray::read_ptr());
    }

    uint8_t* write_ptr() {
      return reinterpret_cast<uint8_t*>(NArray::write_ptr());
    }
  };
}

namespace numo {
  class Int32: public NArray {
  public:
    Int32(VALUE v) {
      construct_value(numo_cInt32, v);
    }

    template<std::size_t N>
    Int32(const size_t(&shape)[N]) {
      construct_shape<N>(numo_cInt32, shape);
    }

    const int32_t* read_ptr() {
      return reinterpret_cast<const int32_t*>(NArray::read_ptr());
    }

    int32_t* write_ptr() {
      return reinterpret_cast<int32_t*>(NArray::write_ptr());
    }
  };
}

namespace numo {
  class Int64: public NArray {
  public:
    Int64(VALUE v) {
      construct_value(numo_cInt64, v);
    }

    template<std::size_t N>
    Int64(const size_t(&shape)[N]) {
      construct_shape<N>(numo_cInt64, shape);
    }

    const int64_t* read_ptr() {
      return reinterpret_cast<const int64_t*>(NArray::read_ptr());
    }

    int64_t* write_ptr() {
      return reinterpret_cast<int64_t*>(NArray::write_ptr());
    }
  };
}

namespace Rice::detail {
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
}

size_t check_shape(numo::NArray objects, size_t k);
