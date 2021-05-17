#include <faiss/Index.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexHNSW.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/IndexLSH.h>
#include <faiss/IndexScalarQuantizer.h>
#include <faiss/IndexPQ.h>
#include <faiss/IndexIVFPQ.h>
#include <faiss/IndexIVFPQR.h>
#include <faiss/index_io.h>
#include <faiss/AutoTune.h>

#include "utils.h"

namespace Rice::detail {
  template<>
  struct Type<faiss::MetricType>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<faiss::MetricType>
  {
  public:
    From_Ruby() = default;

    From_Ruby(Arg* arg) : arg_(arg)
    {
    }

    faiss::MetricType convert(VALUE x)
    {
      if (x == Qnil && this->arg_ && this->arg_->hasDefaultValue()) {
        return this->arg_->defaultValue<faiss::MetricType>();
      }

      auto s = Object(x).to_s().str();
      if (s == "inner_product") {
        return faiss::METRIC_INNER_PRODUCT;
      } else if (s == "l2") {
        return faiss::METRIC_L2;
      } else {
        // TODO throw argument error
        throw std::runtime_error("Invalid metric: " + s);
      }
    }

  private:
    Arg* arg_;
  };

  template<>
  struct Type<faiss::ScalarQuantizer::QuantizerType>
  {
    static bool verify()
    {
      return true;
    }
  };

  template<>
  class From_Ruby<faiss::ScalarQuantizer::QuantizerType>
  {
  public:
    faiss::ScalarQuantizer::QuantizerType convert(VALUE x)
    {
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
        // TODO throw argument error
        throw std::runtime_error("Invalid quantizer type: " + s);
      }
    }
  };
}

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
      "_train",
      [](faiss::Index &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        self.train(n, x);
      })
    .define_method(
      "_add",
      [](faiss::Index &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        self.add(n, x);
      })
    .define_method(
      "_search",
      [](faiss::Index &self, int64_t n, Rice::Object o, int64_t k) {
        const float *x = float_array(o);
        float *distances = new float[k * n];
        int64_t *labels = new int64_t[k * n];

        self.search(n, x, k, distances, labels);

        auto dstr = result(distances, k * n);
        auto lstr = result(labels, k * n);

        Rice::Array ret;
        ret.push(dstr);
        ret.push(lstr);
        return ret;
      })
    .define_method(
      "nprobe=",
      [](faiss::Index &self, double val) {
        faiss::ParameterSpace().set_index_parameter(&self, "nprobe", val);
      })
    .define_method(
      "save",
      [](faiss::Index &self, const char *fname) {
        faiss::write_index(&self, fname);
      })
    .define_singleton_function(
      "load",
      [](const char *fname) {
        return faiss::read_index(fname);
      });

  Rice::define_class_under<faiss::IndexFlatL2, faiss::Index>(m, "IndexFlatL2")
    .define_constructor(Rice::Constructor<faiss::IndexFlatL2, int64_t>());

  Rice::define_class_under<faiss::IndexFlatIP, faiss::Index>(m, "IndexFlatIP")
    .define_constructor(Rice::Constructor<faiss::IndexFlatIP, int64_t>());

  Rice::define_class_under<faiss::IndexHNSWFlat, faiss::Index>(m, "IndexHNSWFlat")
    .define_constructor(Rice::Constructor<faiss::IndexHNSWFlat, int, int, faiss::MetricType>(), Rice::Arg("d"), Rice::Arg("M"), Rice::Arg("metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexIVFFlat, faiss::Index>(m, "IndexIVFFlat")
    .define_constructor(Rice::Constructor<faiss::IndexIVFFlat, faiss::Index*, size_t, size_t, faiss::MetricType>(), Rice::Arg("quantizer"), Rice::Arg("d"), Rice::Arg("nlist"), Rice::Arg("metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexLSH, faiss::Index>(m, "IndexLSH")
    .define_constructor(Rice::Constructor<faiss::IndexLSH, int64_t, int>());

  Rice::define_class_under<faiss::IndexScalarQuantizer, faiss::Index>(m, "IndexScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexScalarQuantizer, int, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexPQ, faiss::Index>(m, "IndexPQ")
    .define_constructor(Rice::Constructor<faiss::IndexPQ, int, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFScalarQuantizer, faiss::Index>(m, "IndexIVFScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexIVFScalarQuantizer, faiss::Index*, size_t, size_t, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexIVFPQ, faiss::Index>(m, "IndexIVFPQ")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQ, faiss::Index*, size_t, size_t, size_t, size_t, faiss::MetricType>(), Rice::Arg("quantizer"), Rice::Arg("d"), Rice::Arg("nlist"), Rice::Arg("M"), Rice::Arg("nbits_per_idx"), Rice::Arg("metric") = faiss::METRIC_L2);

  Rice::define_class_under<faiss::IndexIVFPQR, faiss::Index>(m, "IndexIVFPQR")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQR, faiss::Index*, size_t, size_t, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::ParameterSpace>(m, "ParameterSpace")
    .define_constructor(Rice::Constructor<faiss::ParameterSpace>())
    .define_method(
      "set_index_parameter",
      [](faiss::ParameterSpace& self, faiss::Index* index, const std::string& name, double val) {
        self.set_index_parameter(index, name, val);
      });
}
