import faiss
import numpy as np

objects = np.array([
  [1, 1, 2, 1],
  [5, 4, 6, 5],
  [1, 2, 1, 2]
], dtype=np.float32)

kmeans = faiss.Kmeans(4, 2)
kmeans.train(objects)

print(kmeans.centroids)
print(kmeans.d)
print(kmeans.index)
print(kmeans.index.d)
