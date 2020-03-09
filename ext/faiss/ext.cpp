#include <faiss/IndexFlat.h>
#include <faiss/IndexHNSW.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/IndexLSH.h>
#include <faiss/IndexScalarQuantizer.h>
#include <faiss/IndexPQ.h>
#include <faiss/IndexIVFPQ.h>
#include <faiss/IndexIVFPQR.h>

#include <faiss/IndexBinaryFlat.h>
#include <faiss/IndexBinaryIVF.h>
#include <faiss/index_factory.h>

#include <faiss/Clustering.h>
#include <faiss/VectorTransform.h>

#include <rice/Array.hpp>
#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
#include <rice/Module.hpp>

float* float_array(Rice::Object o)
{
  Rice::String s = o.call("to_binary");
  return (float*) s.c_str();
}

uint8_t* uint8_array(Rice::Object o)
{
  Rice::String s = o.call("to_binary");
  return (uint8_t*) s.c_str();
}

// TODO return Numo::SFloat
Rice::String result(float* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(float)));
}

// TODO return Numo::UInt8
Rice::String result(uint8_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(uint8_t)));
}

// TODO return Numo::Int32
Rice::String result(int32_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(int32_t)));
}

// TODO return Numo::Int64
Rice::String result(int64_t* ptr, int64_t length)
{
  return Rice::String(std::string((char*) ptr, length * sizeof(int64_t)));
}

extern "C"
void Init_ext()
{
  Rice::Module rb_mFaiss = Rice::define_module("Faiss")
    .define_singleton_method(
      "index_binary_factory",
      *[](int d, const char *description) {
        return faiss::index_binary_factory(d, description);
      });

  Rice::define_class_under<faiss::Index>(rb_mFaiss, "Index")
    .define_method(
      "d",
      *[](faiss::Index &self) {
        return self.d;
      })
    .define_method(
      "trained?",
      *[](faiss::Index &self) {
        return self.is_trained;
      })
    .define_method(
      "ntotal",
      *[](faiss::Index &self) {
        return self.ntotal;
      })
    .define_method(
      "_train",
      *[](faiss::Index &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        self.train(n, x);
      })
    .define_method(
      "_add",
      *[](faiss::Index &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        self.add(n, x);
      })
    .define_method(
      "_search",
      *[](faiss::Index &self, int64_t n, Rice::Object o, int64_t k) {
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
      });

  Rice::define_class_under<faiss::IndexBinary>(rb_mFaiss, "IndexBinary")
    .define_method(
      "d",
      *[](faiss::Index &self) {
        return self.d;
      })
    .define_method(
      "trained?",
      *[](faiss::IndexBinary &self) {
        return self.is_trained;
      })
    .define_method(
      "ntotal",
      *[](faiss::IndexBinary &self) {
        return self.ntotal;
      })
    .define_method(
      "_train",
      *[](faiss::IndexBinary &self, int64_t n, Rice::Object o) {
        const uint8_t *x = uint8_array(o);
        self.train(n, x);
      })
    .define_method(
      "_add",
      *[](faiss::IndexBinary &self, int64_t n, Rice::Object o) {
        const uint8_t *x = uint8_array(o);
        self.add(n, x);
      })
    .define_method(
      "_search",
      *[](faiss::IndexBinary &self, int64_t n, Rice::Object o, int64_t k) {
        const uint8_t *x = uint8_array(o);
        int32_t *distances = new int32_t[k * n];
        int64_t *labels = new int64_t[k * n];

        self.search(n, x, k, distances, labels);

        auto dstr = result(distances, k * n);
        auto lstr = result(labels, k * n);

        Rice::Array ret;
        ret.push(dstr);
        ret.push(lstr);
        return ret;
      });

  Rice::define_class_under<faiss::IndexFlatL2, faiss::Index>(rb_mFaiss, "IndexFlatL2")
    .define_constructor(Rice::Constructor<faiss::IndexFlatL2, int64_t>());

  Rice::define_class_under<faiss::IndexFlatIP, faiss::Index>(rb_mFaiss, "IndexFlatIP")
    .define_constructor(Rice::Constructor<faiss::IndexFlatIP, int64_t>());

  Rice::define_class_under<faiss::IndexHNSWFlat, faiss::Index>(rb_mFaiss, "IndexHNSWFlat")
    .define_constructor(Rice::Constructor<faiss::IndexHNSWFlat, int, int>());

  Rice::define_class_under<faiss::IndexIVFFlat, faiss::Index>(rb_mFaiss, "IndexIVFFlat")
    .define_constructor(Rice::Constructor<faiss::IndexIVFFlat, faiss::Index*, size_t, size_t>());

  Rice::define_class_under<faiss::IndexLSH, faiss::Index>(rb_mFaiss, "IndexLSH")
    .define_constructor(Rice::Constructor<faiss::IndexLSH, int64_t, int>());

  Rice::define_class_under<faiss::IndexScalarQuantizer, faiss::Index>(rb_mFaiss, "IndexScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexScalarQuantizer, int, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexPQ, faiss::Index>(rb_mFaiss, "IndexPQ")
    .define_constructor(Rice::Constructor<faiss::IndexPQ, int, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFScalarQuantizer, faiss::Index>(rb_mFaiss, "IndexIVFScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexIVFScalarQuantizer, faiss::Index*, size_t, size_t, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexIVFPQ, faiss::Index>(rb_mFaiss, "IndexIVFPQ")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQ, faiss::Index*, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFPQR, faiss::Index>(rb_mFaiss, "IndexIVFPQR")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQR, faiss::Index*, size_t, size_t, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::IndexBinaryFlat, faiss::IndexBinary>(rb_mFaiss, "IndexBinaryFlat")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryFlat, int64_t>());

  Rice::define_class_under<faiss::IndexBinaryIVF, faiss::IndexBinary>(rb_mFaiss, "IndexBinaryIVF")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryIVF, faiss::IndexBinary*, size_t, size_t>());

  Rice::define_class_under<faiss::Clustering>(rb_mFaiss, "Kmeans")
    .define_constructor(Rice::Constructor<faiss::Clustering, int, int>())
    .define_method(
      "d",
      *[](faiss::Clustering &self) {
        return self.d;
      })
    .define_method(
      "k",
      *[](faiss::Clustering &self) {
        return self.k;
      })
    .define_method(
      "_centroids",
      *[](faiss::Clustering &self) {
        float *centroids = new float[self.k * self.d];
        for (size_t i = 0; i < self.centroids.size(); i++) {
          centroids[i] = self.centroids[i];
        }
        return result(centroids, self.k * self.d);
      })
    .define_method(
      "_train",
      *[](faiss::Clustering &self, int64_t n, Rice::Object o, faiss::Index & index) {
        const float *x = float_array(o);
        self.train(n, x, index);
      });

  Rice::define_class_under<faiss::PCAMatrix>(rb_mFaiss, "PCAMatrix")
    .define_constructor(Rice::Constructor<faiss::PCAMatrix, int, int>())
    .define_method(
      "d_in",
      *[](faiss::PCAMatrix &self) {
        return self.d_in;
      })
    .define_method(
      "d_out",
      *[](faiss::PCAMatrix &self) {
        return self.d_out;
      })
    .define_method(
      "_train",
      *[](faiss::PCAMatrix &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        self.train(n, x);
      })
    .define_method(
      "_apply",
      *[](faiss::PCAMatrix &self, int64_t n, Rice::Object o) {
        const float *x = float_array(o);
        float* res = self.apply(n, x);
        return result(res, n * self.d_out);
      });

  Rice::define_class_under<faiss::ProductQuantizer>(rb_mFaiss, "ProductQuantizer")
    .define_constructor(Rice::Constructor<faiss::ProductQuantizer, size_t, size_t, size_t>())
    .define_method(
      "d",
      *[](faiss::ProductQuantizer &self) {
        return self.d;
      })
    .define_method(
      "m",
      *[](faiss::ProductQuantizer &self) {
        return self.M;
      })
    .define_method(
      "_train",
      *[](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const float *x = float_array(o);
        self.train(n, x);
      })
    .define_method(
      "_compute_codes",
      *[](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const float *x = float_array(o);
        uint8_t *codes = new uint8_t[n * self.M];
        self.compute_codes(x, codes, n);
        return result(codes, n * self.M);
      })
    .define_method(
      "_decode",
      *[](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const uint8_t *codes = uint8_array(o);
        float *x = new float[n * self.d];
        self.decode(codes, x, n);
        return result(x, n * self.d);
      });
}
