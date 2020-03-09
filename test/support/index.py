import faiss
import numpy as np

d = 64
nb = 100000
nq = 10000

np.random.seed(1234)
xb = np.random.random((nb, d)).astype('float32')
xb[:, 0] += np.arange(nb) / 1000.
xq = np.random.random((nq, d)).astype('float32')
xq[:, 0] += np.arange(nq) / 1000.

index = faiss.IndexFlatL2(d)
print(index.is_trained)
index.add(xb)
print(index.ntotal)

k = 4
D, I = index.search(xb[:5], k)
print(I)
print(D)
D, I = index.search(xq, k)
print(I[:5])
print(I[-5:])

faiss.write_index(index, "/tmp/index.bin")
index2 = faiss.read_index("/tmp/index.bin")
print(index2.ntotal)
