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
      "_train",
      [](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const float *x = float_array(o);
        self.train(n, x);
      })
    .define_method(
      "_compute_codes",
      [](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const float *x = float_array(o);
        uint8_t *codes = new uint8_t[n * self.M];
        self.compute_codes(x, codes, n);
        return result(codes, n * self.M);
      })
    .define_method(
      "_decode",
      [](faiss::ProductQuantizer &self, int n, Rice::Object o) {
        const uint8_t *codes = uint8_array(o);
        float *x = new float[n * self.d];
        self.decode(codes, x, n);
        return result(x, n * self.d);
      })
    .define_method(
      "save",
      [](faiss::ProductQuantizer &self, const char *fname) {
        faiss::write_ProductQuantizer(&self, fname);
      })
    .define_singleton_function(
      "load",
      [](const char *fname) {
        return faiss::read_ProductQuantizer(fname);
      });
}
