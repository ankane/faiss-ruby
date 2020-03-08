import faiss
import numpy as np

mt = np.random.rand(1000, 40).astype('float32')
mat = faiss.PCAMatrix(40, 10)
mat.train(mt)
tr = mat.apply_py(mt)

print((tr ** 2).sum(0))
