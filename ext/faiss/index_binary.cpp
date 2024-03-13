#include <faiss/IndexBinary.h>
#include <faiss/IndexBinaryFlat.h>
#include <faiss/IndexBinaryIVF.h>
#include <faiss/index_factory.h>
#include <faiss/index_io.h>

#include "utils.h"

void init_index_binary(Rice::Module& m) {
  Rice::define_class_under<faiss::IndexBinary>(m, "IndexBinary")
    .define_method(
      "d",
      [](faiss::IndexBinary &self) {
        return self.d;
      })
    .define_method(
      "trained?",
      [](faiss::IndexBinary &self) {
        return self.is_trained;
      })
    .define_method(
      "ntotal",
      [](faiss::IndexBinary &self) {
        return self.ntotal;
      })
    .define_method(
      "train",
      [](faiss::IndexBinary &self, numo::UInt8 objects) {
        auto n = check_shape(objects, self.d / 8);
        self.train(n, objects.read_ptr());
      })
    .define_method(
      "add",
      [](faiss::IndexBinary &self, numo::UInt8 objects) {
        auto n = check_shape(objects, self.d / 8);
        self.add(n, objects.read_ptr());
      })
    .define_method(
      "search",
      [](faiss::IndexBinary &self, numo::UInt8 objects, size_t k) {
        auto n = check_shape(objects, self.d / 8);

        auto distances = numo::Int32({n, k});
        auto labels = numo::Int64({n, k});

        self.search(n, objects.read_ptr(), k, distances.write_ptr(), labels.write_ptr());

        Rice::Array ret;
        ret.push(distances);
        ret.push(labels);
        return ret;
      })
    .define_method(
      "save",
      [](faiss::IndexBinary &self, Rice::String fname) {
        faiss::write_index_binary(&self, fname.c_str());
      })
    .define_singleton_function(
      "load",
      [](Rice::String fname) {
        return faiss::read_index_binary(fname.c_str());
      }, Rice::Return().takeOwnership());

  Rice::define_class_under<faiss::IndexBinaryFlat, faiss::IndexBinary>(m, "IndexBinaryFlat")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryFlat, int64_t>());

  Rice::define_class_under<faiss::IndexBinaryIVF, faiss::IndexBinary>(m, "IndexBinaryIVF")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryIVF, faiss::IndexBinary*, size_t, size_t>());

  m.define_singleton_function(
    "index_binary_factory",
    [](int d, Rice::String description) {
      return faiss::index_binary_factory(d, description.c_str());
    }, Rice::Return().takeOwnership());
}
