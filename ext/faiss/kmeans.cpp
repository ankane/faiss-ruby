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
      "_centroids",
      [](faiss::Clustering &self) {
        float *centroids = new float[self.k * self.d];
        for (size_t i = 0; i < self.centroids.size(); i++) {
          centroids[i] = self.centroids[i];
        }
        return result(centroids, self.k * self.d);
      })
    .define_method(
      "_train",
      [](faiss::Clustering &self, int64_t n, Rice::Object o, faiss::Index & index) {
        const float *x = float_array(o);
        self.train(n, x, index);
      });
}
