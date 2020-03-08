import faiss
import numpy as np

objects = np.array([
  [1, 1, 2, 1],
  [5, 4, 6, 5],
  [1, 2, 1, 2]
], dtype=np.float32)

index = faiss.IndexLSH(4, 2)
index.add(objects)
distances, ids = index.search(objects, 3)

print(distances)
print(ids)
