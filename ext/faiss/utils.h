#pragma once

#include <rice/rice.hpp>
#include <numo/narray.h>

namespace numo {
  class NArray {
  public:
    VALUE value() const;
    size_t ndim();
    size_t* shape();

  protected:
    void construct_value(VALUE dtype, VALUE v);
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

    const float* read_ptr();
    float* write_ptr();
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

    const uint8_t* read_ptr();
    uint8_t* write_ptr();
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

    const int32_t* read_ptr();
    int32_t* write_ptr();
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

    const int64_t* read_ptr();
    int64_t* write_ptr();
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
