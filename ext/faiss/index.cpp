#include <string>

#include <faiss/AutoTune.h>
#include <faiss/Index.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexHNSW.h>
#include <faiss/IndexIDMap.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/IndexLSH.h>
#include <faiss/IndexScalarQuantizer.h>
#include <faiss/IndexPQ.h>
#include <faiss/IndexIVFPQ.h>
#include <faiss/IndexIVFPQR.h>
#include <faiss/index_io.h>
#include <rice/rice.hpp>
#include <rice/stl.hpp>

#include "numo.hpp"
#include "utils.h"

namespace Rice::detail {
  template<>
  struct Type<faiss::MetricType> {
    static bool verify() { return true; }
  };

  template<>
  class From_Ruby<faiss::MetricType> {
  public:
    From_Ruby() = default;

    From_Ruby(Arg* arg) : arg_(arg) {}

    Convertible is_convertible(VALUE value) { return Convertible::Cast; }

    faiss::MetricType convert(VALUE x) {
      if (x == Qnil && this->arg_ && this->arg_->hasDefaultValue()) {
        return this->arg_->defaultValue<faiss::MetricType>();
      }

      auto s = Object(x).to_s().str();
      if (s == "inner_product") {
        return faiss::METRIC_INNER_PRODUCT;
      } else if (s == "l2") {
        return faiss::METRIC_L2;
      } else {
        throw Rice::Exception(rb_eArgError, "Invalid metric: %s", s.c_str());
      }
    }

  private:
    Arg* arg_;
  };

  template<>
  struct Type<faiss::ScalarQuantizer::QuantizerType> {
    static bool verify() { return true; }
  };

  template<>
  class From_Ruby<faiss::ScalarQuantizer::QuantizerType> {
  public:
    Convertible is_convertible(VALUE value) { return Convertible::Cast; }

    faiss::ScalarQuantizer::QuantizerType convert(VALUE x) {
      auto s = Object(x).to_s().str();
      if (s == "qt_8bit") {
        return faiss::ScalarQuantizer::QT_8bit;
      } else if (s == "qt_4bit") {
        return faiss::ScalarQuantizer::QT_4bit;
      } else if (s == "qt_8bit_uniform") {
        return faiss::ScalarQuantizer::QT_8bit_uniform;
      } else if (s == "qt_4bit_uniform") {
        return faiss::ScalarQuantizer::QT_4bit_uniform;
      } else if (s == "qt_fp16") {
        return faiss::ScalarQuantizer::QT_fp16;
      } else if (s == "qt_8bit_direct") {
        return faiss::ScalarQuantizer::QT_8bit_direct;
      } else if (s == "qt_6bit") {
        return faiss::ScalarQuantizer::QT_6bit;
      } else {
        throw Rice::Exception(rb_eArgError, "Invalid quantizer type: %s", s.c_str());
      }
    }
  };
} // namespace Rice::detail

void init_index(Rice::Module& m) {
  Rice::define_class_under<faiss::Index>(m, "Index")
    .define_method(
      "d",
      [](faiss::Index &self) {
        return self.d;
      })
    .define_method(
      "trained?",
      [](faiss::Index &self) {
        return self.is_trained;
      })
    .define_method(
      "ntotal",
      [](faiss::Index &self) {
        return self.ntotal;
      })
    .define_method(
      "train",
      [](faiss::Index &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d);
        self.train(n, objects.read_ptr());
      })
    .define_method(
      "add",
      [](faiss::Index &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d);
        self.add(n, objects.read_ptr());
      })
    .define_method(
      "add_with_ids",
      [](faiss::Index &self, numo::SFloat objects, numo::Int64 ids) {
        auto n = check_shape(objects, self.d);
        if (ids.ndim() != 1 || ids.shape()[0] != n) {
          throw Rice::Exception(rb_eArgError, "expected ids to be 1d array with size %d", n);
        }
        self.add_with_ids(n, objects.read_ptr(), ids.read_ptr());
      })
    .define_method(
      "search",
      [](faiss::Index &self, numo::SFloat objects, size_t k) {
        auto n = check_shape(objects, self.d);

        auto distances = numo::SFloat({n, k});
        auto labels = numo::Int64({n, k});

        without_gvl([&] {
          self.search(n, objects.read_ptr(), k, distances.write_ptr(), labels.write_ptr());
        });

        Rice::Array ret;
        ret.push(distances);
        ret.push(labels);
        return ret;
      })
    .define_method(
      "nprobe=",
      [](faiss::Index &self, double val) {
        faiss::ParameterSpace().set_index_parameter(&self, "nprobe", val);
      })
    .define_method(
      "save",
      [](faiss::Index &self, Rice::String fname) {
        faiss::write_index(&self, fname.c_str());
      })
    .define_singleton_function(
      "load",
      [](Rice::String fname) {
        return faiss::read_index(fname.c_str());
      }, Rice::Return().takeOwnership());

  Rice::define_class_under<faiss::IndexFlatL2, faiss::Index>(m, "IndexFlatL2")
    .define_constructor(Rice::Constructor<faiss::IndexFlatL2, int64_t>());

  Rice::define_class_under<faiss::IndexFlatIP, faiss::Index>(m, "IndexFlatIP")
    .define_constructor(Rice::Constructor<faiss::IndexFlatIP, int64_t>());

  Rice::define_class_under<faiss::IndexHNSWFlat, faiss::Index>(m, "IndexHNSWFlat")
    .define_constructor(Rice::Constructor<faiss::IndexHNSWFlat, int, int, faiss::MetricType>(), Rice::Arg("_d"), Rice::Arg("_M"), Rice::Arg("_metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexIVFFlat, faiss::Index>(m, "IndexIVFFlat")
    .define_constructor(Rice::Constructor<faiss::IndexIVFFlat, faiss::Index*, size_t, size_t, faiss::MetricType>(), Rice::Arg("_quantizer"), Rice::Arg("_d"), Rice::Arg("_nlist"), Rice::Arg("_metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexLSH, faiss::Index>(m, "IndexLSH")
    .define_constructor(Rice::Constructor<faiss::IndexLSH, int64_t, int>());

  Rice::define_class_under<faiss::IndexScalarQuantizer, faiss::Index>(m, "IndexScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexScalarQuantizer, int, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexPQ, faiss::Index>(m, "IndexPQ")
    .define_constructor(Rice::Constructor<faiss::IndexPQ, int, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFScalarQuantizer, faiss::Index>(m, "IndexIVFScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexIVFScalarQuantizer, faiss::Index*, size_t, size_t, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexIVFPQ, faiss::Index>(m, "IndexIVFPQ")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQ, faiss::Index*, size_t, size_t, size_t, size_t, faiss::MetricType>(), Rice::Arg("_quantizer"), Rice::Arg("_d"), Rice::Arg("_nlist"), Rice::Arg("_M"), Rice::Arg("_nbits_per_idx"), Rice::Arg("_metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexIVFPQR, faiss::Index>(m, "IndexIVFPQR")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQR, faiss::Index*, size_t, size_t, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::ParameterSpace>(m, "ParameterSpace")
    .define_constructor(Rice::Constructor<faiss::ParameterSpace>())
    .define_method(
      "set_index_parameter",
      [](faiss::ParameterSpace& self, faiss::Index* index, const std::string& name, double val) {
        self.set_index_parameter(index, name, val);
      });

  Rice::define_class_under<faiss::IndexIDMap, faiss::Index>(m, "IndexIDMap")
    .define_constructor(Rice::Constructor<faiss::IndexIDMap, faiss::Index*>());
}
