#include <algorithm>
#include <cstddef>

#include <faiss/Clustering.h>
#include <faiss/IndexFlat.h>
#include <rice/rice.hpp>

#include "numo.hpp"
#include "utils_rb.h"

void init_kmeans(Rice::Module& m) {
  Rice::define_class_under<faiss::Clustering>(m, "Kmeans")
    .define_constructor(Rice::Constructor<faiss::Clustering, int, int>())
    .define_method(
      "index",
      [](Rice::Object self) {
        return self.attr_get("@index");
      })
    .define_method(
      "d",
      [](faiss::Clustering& self) {
        return self.d;
      })
    .define_method(
      "k",
      [](faiss::Clustering& self) {
        return self.k;
      })
    .define_method(
      "centroids",
      [](faiss::Clustering& self) {
        numo::SFloat centroids{{self.k, self.d}};
        std::ranges::copy(self.centroids, centroids.write_ptr());
        return centroids;
      })
    .define_method(
      "train",
      [](Rice::Object rb_self, numo::SFloat objects) {
        auto& self = *Rice::Data_Object<faiss::Clustering>{rb_self};
        size_t n = check_shape(objects, self.d);
        faiss::IndexFlatL2 index(self.d);
        rb_self.iv_set("@index", index);
        self.train(n, objects.read_ptr(), index);
      });
}
