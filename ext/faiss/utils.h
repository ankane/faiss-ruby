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
    VALUE _value;
  };

  class SFloat: public NArray {
  public:
    SFloat(VALUE v) {
      v = rb_funcall(numo_cSFloat, rb_intern("cast"), 1, v);
      if (nary_check_contiguous(v) == Qfalse) {
        v = nary_dup(v);
      }
      this->_value = v;
    }

    SFloat(std::initializer_list<size_t> s) {
      auto shape = ALLOCA_N(size_t, s.size());
      size_t i = 0;
      for (auto& v : s) {
        shape[i++] = v;
      }
      this->_value = rb_narray_new(numo_cSFloat, s.size(), shape);
    }

    const float* read_ptr();
    float* write_ptr();
  };
}

namespace numo {
  class UInt8: public NArray {
  public:
    UInt8(VALUE v) {
      v = rb_funcall(numo_cUInt8, rb_intern("cast"), 1, v);
      if (nary_check_contiguous(v) == Qfalse) {
        v = nary_dup(v);
      }
      this->_value = v;
    }

    UInt8(std::initializer_list<size_t> s) {
      auto shape = ALLOCA_N(size_t, s.size());
      size_t i = 0;
      for (auto& v : s) {
        shape[i++] = v;
      }
      this->_value = rb_narray_new(numo_cUInt8, s.size(), shape);
    }

    const uint8_t* read_ptr();
    uint8_t* write_ptr();
  };
}

namespace numo {
  class Int32: public NArray {
  public:
    Int32(VALUE v) {
      v = rb_funcall(numo_cInt32, rb_intern("cast"), 1, v);
      if (nary_check_contiguous(v) == Qfalse) {
        v = nary_dup(v);
      }
      this->_value = v;
    }

    Int32(std::initializer_list<size_t> s) {
      auto shape = ALLOCA_N(size_t, s.size());
      size_t i = 0;
      for (auto& v : s) {
        shape[i++] = v;
      }
      this->_value = rb_narray_new(numo_cInt32, s.size(), shape);
    }

    const int32_t* read_ptr();
    int32_t* write_ptr();
  };
}

namespace numo {
  class Int64: public NArray {
  public:
    Int64(VALUE v) {
      v = rb_funcall(numo_cInt64, rb_intern("cast"), 1, v);
      if (nary_check_contiguous(v) == Qfalse) {
        v = nary_dup(v);
      }
      this->_value = v;
    }

    Int64(std::initializer_list<size_t> s) {
      auto shape = ALLOCA_N(size_t, s.size());
      size_t i = 0;
      for (auto& v : s) {
        shape[i++] = v;
      }
      this->_value = rb_narray_new(numo_cInt64, s.size(), shape);
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
