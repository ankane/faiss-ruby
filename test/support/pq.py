import faiss
import numpy as np

x = np.random.rand(10000, 32).astype('float32')

pq = faiss.ProductQuantizer(32, 4, 8)
pq.train(x)

codes = pq.compute_codes(x)
x2 = pq.decode(codes)

avg_relative_error = ((x - x2)**2).sum() / (x ** 2).sum()
print(avg_relative_error)
