#include <faiss/Clustering.h>
#include <faiss/IndexFlat.h>
#include <rice/rice.hpp>

#include "numo.hpp"
#include "utils.h"

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
      [](faiss::Clustering &self) {
        return self.d;
      })
    .define_method(
      "k",
      [](faiss::Clustering &self) {
        return self.k;
      })
    .define_method(
      "centroids",
      [](faiss::Clustering &self) {
        auto centroids = numo::SFloat({self.k, self.d});
        memcpy(centroids.write_ptr(), self.centroids.data(), self.centroids.size() * sizeof(float));
        return centroids;
      })
    .define_method(
      "train",
      [](Rice::Object rb_self, numo::SFloat objects) {
        auto &self = *Rice::Data_Object<faiss::Clustering>{rb_self};
        auto n = check_shape(objects, self.d);
        auto index = faiss::IndexFlatL2(self.d);
        rb_self.iv_set("@index", index);
        self.train(n, objects.read_ptr(), index);
      });
}
