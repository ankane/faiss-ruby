#include <faiss/IndexBinary.h>
#include <faiss/IndexBinaryFlat.h>
#include <faiss/IndexBinaryIVF.h>
#include <faiss/index_factory.h>
#include <faiss/index_io.h>

#include <rice/Array.hpp>
#include <rice/Constructor.hpp>
#include <rice/Module.hpp>

#include "utils.h"

void init_index_binary(Rice::Module& m) {
  Rice::define_class_under<faiss::IndexBinary>(m, "IndexBinary")
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
      })
    .define_method(
      "save",
      *[](faiss::IndexBinary &self, const char *fname) {
        faiss::write_index_binary(&self, fname);
      })
    .define_singleton_method(
      "load",
      *[](const char *fname) {
        return faiss::read_index_binary(fname);
      });

  Rice::define_class_under<faiss::IndexBinaryFlat, faiss::IndexBinary>(m, "IndexBinaryFlat")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryFlat, int64_t>());

  Rice::define_class_under<faiss::IndexBinaryIVF, faiss::IndexBinary>(m, "IndexBinaryIVF")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryIVF, faiss::IndexBinary*, size_t, size_t>());

  m.define_singleton_method(
    "index_binary_factory",
    *[](int d, const char *description) {
      return faiss::index_binary_factory(d, description);
    });
}
