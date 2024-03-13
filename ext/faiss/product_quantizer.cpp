#include <faiss/impl/ProductQuantizer.h>
#include <faiss/index_io.h>

#include "utils.h"

void init_product_quantizer(Rice::Module& m) {
  Rice::define_class_under<faiss::ProductQuantizer>(m, "ProductQuantizer")
    .define_constructor(Rice::Constructor<faiss::ProductQuantizer, size_t, size_t, size_t>())
    .define_method(
      "d",
      [](faiss::ProductQuantizer &self) {
        return self.d;
      })
    .define_method(
      "m",
      [](faiss::ProductQuantizer &self) {
        return self.M;
      })
    .define_method(
      "train",
      [](faiss::ProductQuantizer &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d);
        self.train(n, objects.read_ptr());
      })
    .define_method(
      "compute_codes",
      [](faiss::ProductQuantizer &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d);

        auto codes = numo::UInt8({n, self.M});
        self.compute_codes(objects.read_ptr(), codes.write_ptr(), n);
        return codes;
      })
    .define_method(
      "decode",
      [](faiss::ProductQuantizer &self, numo::UInt8 objects) {
        auto n = check_shape(objects, self.M);

        auto x = numo::SFloat({n, self.d});
        self.decode(objects.read_ptr(), x.write_ptr(), n);
        return x;
      })
    .define_method(
      "save",
      [](faiss::ProductQuantizer &self, Rice::String fname) {
        faiss::write_ProductQuantizer(&self, fname.c_str());
      })
    .define_singleton_function(
      "load",
      [](Rice::String fname) {
        return faiss::read_ProductQuantizer(fname.c_str());
      }, Rice::Return().takeOwnership());
}
