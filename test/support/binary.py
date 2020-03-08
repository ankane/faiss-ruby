import faiss
import numpy as np

objects = np.array([
  [1, 1, 2, 1],
  [5, 4, 6, 5],
  [1, 2, 1, 2]
], dtype=np.uint8)

quantizer = faiss.IndexBinaryFlat(32)
index = faiss.IndexBinaryIVF(quantizer, 32, 2)
index.train(objects)
index.add(objects)
distances, ids = index.search(objects, 3)

print(distances)
print(ids)
