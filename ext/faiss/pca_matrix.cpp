#include <faiss/VectorTransform.h>

#include <rice/Constructor.hpp>
#include <rice/Module.hpp>

#include "utils.h"

void init_pca_matrix(Rice::Module& m) {
  Rice::define_class_under<faiss::PCAMatrix>(m, "PCAMatrix")
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
}
