require_relative "test_helper"

class IndexTest < Minitest::Test
  def test_index
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

  def test_view
    objects = Numo::SFloat.new(100, 1).seq[50..-1, true]
    index = Faiss::IndexFlatL2.new(1)
    index.add(objects)
    d, i = index.search([[0]], 1)
    assert_equal [0], i[true, 0].to_a
    assert_equal [2500], d[true, 0].to_a
  end

  def test_non_contiguous
    objects = Numo::SFloat.new(100, 1).seq.reverse[0...50, true]
    refute objects.contiguous?
    index = Faiss::IndexFlatL2.new(1)
    index.add(objects)
    d, i = index.search([[0]], 1)
    assert_equal [49], i[true, 0].to_a
    assert_equal [2500], d[true, 0].to_a
  end

  def test_bad_dimensions
    index = Faiss::IndexFlatL2.new(3)
    error = assert_raises(ArgumentError) do
      index.add([1])
    end
    assert_equal "expected 2 dimensions, not 1", error.message
  end

  def test_bad_dimensions_view
    index = Faiss::IndexFlatL2.new(3)
    objects = Numo::SFloat.new(1, 3).seq
    error = assert_raises(ArgumentError) do
      index.add(objects.expand_dims(0))
    end
    assert_equal "expected 2 dimensions, not 3", error.message
  end

  def test_bad_shape
    index = Faiss::IndexFlatL2.new(3)
    error = assert_raises(ArgumentError) do
      index.add([[1]])
    end
    assert_equal "expected 2nd dimension to be 3, not 1", error.message
  end

  def test_bad_shape_view
    index = Faiss::IndexFlatL2.new(3)
    objects = Numo::SFloat.cast([[1, 2, 3]])
    error = assert_raises(ArgumentError) do
      index.add(objects[true, 0..1])
    end
    assert_equal "expected 2nd dimension to be 3, not 2", error.message
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

  def test_index_hnsw_flat_inner_product
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexHNSWFlat.new(4, 2, :inner_product)
    index.add(objects)
    distances, ids = index.search(objects, 3)

    assert_equal [[26, 7, 7], [102, 29, 26], [29, 10, 7]], distances.to_a
    assert_equal [[1, 0, 2], [1, 2, 0], [1, 2, 0]], ids.to_a
  end

  def test_invalid_metric
    error = assert_raises do
      Faiss::IndexHNSWFlat.new(4, 2, :bad)
    end
    assert_equal "Invalid metric: bad", error.message
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
    # TODO improve test
    Faiss::IndexScalarQuantizer.new(4, :qt_8bit)
  end

  def test_invalid_quantizer_type
    error = assert_raises do
      Faiss::IndexScalarQuantizer.new(4, :bad)
    end
    assert_equal "Invalid quantizer type: bad", error.message
  end

  def test_index_pq
    # TODO improve test
    Faiss::IndexPQ.new(128, 16, 8)
  end

  def test_index_ivf_scalar_quantizer
    # TODO improve test
    quantizer = Faiss::IndexFlatL2.new(4)
    Faiss::IndexIVFScalarQuantizer.new(quantizer, 4, 8, :qt_8bit)
  end

  def test_index_ivfpq
    # TODO improve test
    quantizer = Faiss::IndexPQ.new(128, 16, 8)
    Faiss::IndexIVFPQ.new(quantizer, 128, 2, 16, 8)
  end

  def test_index_ivfpqr
    # TODO improve test
    quantizer = Faiss::IndexPQ.new(128, 16, 8)
    Faiss::IndexIVFPQR.new(quantizer, 128, 2, 16, 8, 2, 2)
  end

  def test_add_with_ids_flat_l2
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101, 102]
    index = Faiss::IndexFlatL2.new(4)
    error = assert_raises do
      index.add_with_ids(objects, ids)
    end
    assert_match "add_with_ids not implemented for this type of index", error.message
  end

  def test_add_with_ids_id_map
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101, 102]
    index = Faiss::IndexFlatL2.new(4)
    index2 = Faiss::IndexIDMap.new(index)
    index2.add_with_ids(objects, ids)
    distances, ids = index2.search(objects, 3)

    assert_equal [[0, 3, 57], [0, 54, 57], [0, 3, 54]], distances.to_a
    assert_equal [[100, 102, 101], [101, 102, 100], [102, 100, 101]], ids.to_a
  end

  def test_add_with_ids_id_map2
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101, 102]
    index = Faiss::IndexFlatL2.new(4)
    index2 = Faiss::IndexIDMap2.new(index)
    index2.add_with_ids(objects, ids)
    distances, ids = index2.search(objects, 3)
    assert_equal [[0, 3, 57], [0, 54, 57], [0, 3, 54]], distances.to_a
    assert_equal [[100, 102, 101], [101, 102, 100], [102, 100, 101]], ids.to_a
  end

  def test_reconstruct_id_map2
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101, 102]
    index = Faiss::IndexFlatL2.new(4)
    index2 = Faiss::IndexIDMap2.new(index)
    index2.add_with_ids(objects, ids)
    assert_equal [1, 1, 2, 1], index2.reconstruct(100).to_a
    assert_equal [5, 4, 6, 5], index2.reconstruct(101).to_a
    assert_equal [1, 2, 1, 2], index2.reconstruct(102).to_a
  end

  def test_add_with_ids_ivf_flat
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101, 102]
    quantizer = Faiss::IndexFlatL2.new(4)
    index = Faiss::IndexIVFFlat.new(quantizer, 4, 2)
    index.train(objects)
    index.add_with_ids(objects, ids)
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 3, max_float], [0, max_float, max_float], [0, 3, max_float]], distances.to_a
    assert_equal [[100, 102, -1], [101, -1, -1], [102, 100, -1]], ids.to_a
  end

  def test_add_with_ids_size_mismatch
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    ids = [100, 101]
    index = Faiss::IndexFlatL2.new(4)
    error = assert_raises do
      index.add_with_ids(objects, ids)
    end
    assert_match "expected ids to be 1d array with size 3", error.message
  end

  def test_add_frozen
    index = Faiss::IndexFlatL2.new(4)
    index.freeze
    assert_raises(FrozenError) do
      index.add([[1, 1, 2, 1]])
    end
  end

  def test_add_with_ids_frozen
    index = Faiss::IndexFlatL2.new(4)
    index.freeze
    assert_raises(FrozenError) do
      index.add_with_ids([[1, 1, 2, 1]], [100])
    end
  end

  def test_search_frozen
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)
    index.freeze
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 3, 57], [0, 54, 57], [0, 3, 54]], distances.to_a
    assert_equal [[0, 2, 1], [1, 2, 0], [2, 0, 1]], ids.to_a
  end

  def test_reconstruct
    objects = Numo::SFloat.cast([
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ])
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)

    (0..2).each do |i|
      assert_equal objects[i, 0..], index.reconstruct(i)
    end
  end

  def test_reconstruct_n
    objects = Numo::SFloat.cast([
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2],
      [3, 3, 3, 3],
      [7, 8, 9, 10]
    ])
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)

    # Reconstruct all vectors
    recons = index.reconstruct_n(0, 5)
    assert_equal [5, 4], recons.shape
    assert_equal objects.to_a, recons.to_a

    # Reconstruct subset from middle
    recons = index.reconstruct_n(1, 3)
    assert_equal [3, 4], recons.shape
    assert_equal objects[1..3, true].to_a, recons.to_a

    # Reconstruct single vector
    recons = index.reconstruct_n(2, 1)
    assert_equal [1, 4], recons.shape
    assert_equal [objects[2, true].to_a], recons.to_a
  end

  def test_reconstruct_batch
    objects = Numo::SFloat.cast([
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2],
      [3, 3, 3, 3],
      [7, 8, 9, 10]
    ])
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)

    # Reconstruct specific vectors by their ids
    keys = Numo::Int64.cast([0, 2, 4])
    recons = index.reconstruct_batch(keys)
    assert_equal [3, 4], recons.shape
    assert_equal objects[0, true].to_a, recons[0, true].to_a
    assert_equal objects[2, true].to_a, recons[1, true].to_a
    assert_equal objects[4, true].to_a, recons[2, true].to_a

    # Reconstruct in different order
    keys = Numo::Int64.cast([4, 1, 3])
    recons = index.reconstruct_batch(keys)
    assert_equal [3, 4], recons.shape
    assert_equal objects[4, true].to_a, recons[0, true].to_a
    assert_equal objects[1, true].to_a, recons[1, true].to_a
    assert_equal objects[3, true].to_a, recons[2, true].to_a

    # Reconstruct single vector
    keys = Numo::Int64.cast([2])
    recons = index.reconstruct_batch(keys)
    assert_equal [1, 4], recons.shape
    assert_equal objects[2, true].to_a, recons[0, true].to_a
  end

  def test_reconstruct_batch_with_ids
    objects = Numo::SFloat.cast([
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ])
    ids = Numo::Int64.cast([100, 101, 102])
    index = Faiss::IndexFlatL2.new(4)
    index2 = Faiss::IndexIDMap2.new(index)
    index2.add_with_ids(objects, ids)

    # Reconstruct using custom ids
    keys = Numo::Int64.cast([100, 102])
    recons = index2.reconstruct_batch(keys)
    assert_equal [2, 4], recons.shape
    assert_equal objects[0, true].to_a, recons[0, true].to_a
    assert_equal objects[2, true].to_a, recons[1, true].to_a
  end

  def test_remove_ids
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexFlatL2.new(4)
    index.add(objects)
    index.remove_ids([0, 2])
    assert_equal 1, index.ntotal
  end

  private

  def max_float # single-precision
    @max_float ||= (2 - 2**-23) * 2**127
  end
end
