#include <faiss/Clustering.h>
#include <faiss/IndexFlat.h>

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
      [](Rice::Object self, numo::SFloat objects) {
        auto self_ptr = Rice::detail::From_Ruby<faiss::Clustering*>().convert(self.value());
        auto n = check_shape(objects, self_ptr->d);
        auto index = faiss::IndexFlatL2(self_ptr->d);
        self.iv_set("@index", Rice::Object(Rice::detail::To_Ruby<faiss::IndexFlatL2>().convert(index)));
        self_ptr->train(n, objects.read_ptr(), index);
      });
}
