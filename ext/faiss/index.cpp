#include <faiss/Index.h>
#include <faiss/index_io.h>

#include <rice/Array.hpp>
#include <rice/Module.hpp>

#include "utils.h"

void init_index(Rice::Module& m) {
  Rice::define_class_under<faiss::Index>(m, "Index")
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
      })
    .define_method(
      "save",
      *[](faiss::Index &self, const char *fname) {
        faiss::write_index(&self, fname);
      })
    .define_singleton_method(
      "load",
      *[](const char *fname) {
        return faiss::read_index(fname);
      });
}
