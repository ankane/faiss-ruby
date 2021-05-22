#include <faiss/Clustering.h>

#include "utils.h"

void init_kmeans(Rice::Module& m) {
  Rice::define_class_under<faiss::Clustering>(m, "Kmeans")
    .define_constructor(Rice::Constructor<faiss::Clustering, int, int>())
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
      "_train",
      [](faiss::Clustering &self, numo::SFloat objects, faiss::Index & index) {
        auto n = check_shape(objects, self.d);
        self.train(n, objects.read_ptr(), index);
      });
}
