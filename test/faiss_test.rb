require_relative "test_helper"

class FaissTest < Minitest::Test
  def test_works
    Numo::NArray.srand(1234)

    d = 64
    nb = 100000
    nq = 10000
    xb = Numo::SFloat.new(nb, d).rand
    xb[true, 0] += Numo::Int64.new(nb).seq / 1000.0
    xq = Numo::SFloat.new(nq, d).rand
    xq[true, 0] += Numo::Int64.new(nq).seq / 1000.0

    index = Faiss::IndexFlatL2.new(d)
    assert index.trained?
    index.add(xb)
    assert_equal nb, index.ntotal

    k = 4
    d, i = index.search(xb[0...5, true], k)
    assert_equal [0, 1, 2, 3, 4], i[true, 0].to_a
    assert_equal [0, 0, 0, 0, 0], d[true, 0].to_a

    # d, i = index.search(xq, k)
    # i[0...5, true]
    # i[-5..-1, true]

    path = "#{Dir.tmpdir}/index.bin"
    index.save(path)
    index2 = Faiss::Index.load(path)
    assert_equal index.ntotal, index2.ntotal
  end

  def test_index_flat_l2
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 3, 57], [0, 54, 57], [0, 3, 54]], distances.to_a
    assert_equal [[0, 2, 1], [1, 2, 0], [2, 0, 1]], ids.to_a
  end

  def test_index_flat_ip
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexFlatIP.new(4)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[26, 7, 7], [102, 29, 26], [29, 10, 7]], distances.to_a
    assert_equal [[1, 2, 0], [1, 2, 0], [1, 2, 0]], ids.to_a
  end

  def test_index_hnsw_flat
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexHNSWFlat.new(4, 2)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 3, 57], [0, 54, 57], [0, 3, 54]], distances.to_a
    assert_equal [[0, 2, 1], [1, 2, 0], [2, 0, 1]], ids.to_a
  end

  def test_index_ivf_flat
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    quantizer = Faiss::IndexFlatL2.new(4)
    index = Faiss::IndexIVFFlat.new(quantizer, 4, 2)
    index.train(objects)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 3, max_float], [0, max_float, max_float], [0, 3, max_float]], distances.to_a
    assert_equal [[0, 2, -1], [1, -1, -1], [2, 0, -1]], ids.to_a
  end

  def test_index_lsh
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexLSH.new(4, 2)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 0, 1], [0, 0, 1], [0, 1, 1]], distances.to_a
    assert_equal [[0, 1, 2], [0, 1, 2], [2, 0, 1]], ids.to_a
  end

  def test_index_scalar_quantizer
    # TODO
  end

  def test_index_pq
    # TODO improve test
    index = Faiss::IndexPQ.new(128, 16, 8)
  end

  def test_index_ivf_scalar_quantizer
    # TODO
  end

  def test_index_ivfpq
    # TODO improve test
    quantizer = Faiss::IndexPQ.new(128, 16, 8)
    index = Faiss::IndexIVFPQ.new(quantizer, 128, 2, 16, 8)
  end

  def test_index_ivfpqr
    # TODO improve test
    quantizer = Faiss::IndexPQ.new(128, 16, 8)
    index = Faiss::IndexIVFPQR.new(quantizer, 128, 2, 16, 8, 2, 2)
  end

  def test_index_binary_flat
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexBinaryFlat.new(32)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 5, 6], [0, 5, 9], [0, 6, 9]], distances.to_a
    assert_equal [[0, 1, 2], [1, 0, 2], [2, 0, 1]], ids.to_a

    path = "#{Dir.tmpdir}/index.bin"
    index.save(path)
    index2 = Faiss::IndexBinary.load(path)
    assert_equal index.ntotal, index2.ntotal
  end

  def test_index_binary_ivf
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    quantizer = Faiss::IndexBinaryFlat.new(32)
    index = Faiss::IndexBinaryIVF.new(quantizer, 32, 2)
    index.train(objects)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 6, max_int], [0, max_int, max_int], [0, 6, max_int]], distances.to_a
    assert_equal [[0, 2, -1], [1, -1, -1], [2, 0, -1]], ids.to_a
  end

  def test_index_binary_factory
    Faiss.index_binary_factory(8, "BIVF32")
  end

  def test_kmeans
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    kmeans = Faiss::Kmeans.new(4, 2)
    kmeans.train(objects)

    assert_equal [[5, 4, 6, 5], [1, 1.5, 1.5, 1.5]], kmeans.centroids.to_a
    assert_equal 4, kmeans.d
    assert_kind_of Faiss::IndexFlatL2, kmeans.index
    assert_equal 4, kmeans.index.d
  end

  def test_pca_matrix
    mt = Numo::SFloat.new(1000, 40).rand
    mat = Faiss::PCAMatrix.new(40, 10)
    mat.train(mt)
    tr = mat.apply(mt)
    assert_operator (tr ** 2).sum(0).to_a.last, :<, 100
  end

  def test_product_quantizer
    x = Numo::SFloat.new(10000, 32).rand
    pq = Faiss::ProductQuantizer.new(32, 4, 8)
    pq.train(x)
    codes = pq.compute_codes(x)
    assert_equal [10000, 4], codes.shape
    assert_kind_of Numo::UInt8, codes
    x2 = pq.decode(codes)
    assert_equal [10000, 32], x2.shape
    assert_kind_of Numo::SFloat, x2
    assert_operator ((x - x2)**2).sum / (x**2).sum, :<, 0.06

    path = "#{Dir.tmpdir}/pq.bin"
    pq.save(path)
    pq = Faiss::ProductQuantizer.load(path)
    x3 = pq.decode(codes)
    assert_equal x2, x3
  end

  private

  def max_float # single-precision
    @max_float ||= (2 - 2**-23) * 2**127
  end

  def max_int
    @max_int ||= 2**31 - 1
  end
end
