#include <cstddef>

#include <faiss/VectorTransform.h>
#include <rice/rice.hpp>

#include "numo.hpp"
#include "utils_rb.h"

void init_pca_matrix(Rice::Module& m) {
  Rice::define_class_under<faiss::PCAMatrix>(m, "PCAMatrix")
    .define_constructor(Rice::Constructor<faiss::PCAMatrix, int, int>())
    .define_method(
      "d_in",
      [](faiss::PCAMatrix& self) {
        return self.d_in;
      })
    .define_method(
      "d_out",
      [](faiss::PCAMatrix& self) {
        return self.d_out;
      })
    .define_method(
      "train",
      [](faiss::PCAMatrix& self, numo::SFloat objects) {
        size_t n = check_shape(objects, self.d_in);
        self.train(n, objects.read_ptr());
      })
    .define_method(
      "apply",
      [](faiss::PCAMatrix& self, numo::SFloat objects) {
        size_t n = check_shape(objects, self.d_in);

        numo::SFloat ary{{n, static_cast<size_t>(self.d_out)}};
        self.apply_noalloc(n, objects.read_ptr(), ary.write_ptr());
        return ary;
      });
}
