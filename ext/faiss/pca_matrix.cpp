#include <faiss/VectorTransform.h>

#include "utils.h"

void init_pca_matrix(Rice::Module& m) {
  Rice::define_class_under<faiss::PCAMatrix>(m, "PCAMatrix")
    .define_constructor(Rice::Constructor<faiss::PCAMatrix, int, int>())
    .define_method(
      "d_in",
      [](faiss::PCAMatrix &self) {
        return self.d_in;
      })
    .define_method(
      "d_out",
      [](faiss::PCAMatrix &self) {
        return self.d_out;
      })
    .define_method(
      "train",
      [](faiss::PCAMatrix &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d_in);
        self.train(n, objects.read_ptr());
      })
    .define_method(
      "apply",
      [](faiss::PCAMatrix &self, numo::SFloat objects) {
        auto n = check_shape(objects, self.d_in);

        float* res = self.apply(n, objects.read_ptr());

        auto ary = numo::SFloat({n, static_cast<size_t>(self.d_out)});
        memcpy(ary.write_ptr(), res, n * self.d_out * sizeof(float));
        return ary;
      });
}
