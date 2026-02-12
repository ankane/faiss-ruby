require_relative "test_helper"

class IndexBinaryTest < Minitest::Test
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

  def test_remove_ids
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexBinaryFlat.new(32)
    index.add(objects)
    assert_equal 1, index.remove_ids([0, 99])
    _, ids = index.search([[1, 2, 1, 2]], 3)

    # changes ids of existing vectors
    # https://github.com/facebookresearch/faiss/wiki/Special-operations-on-indexes#removing-elements-from-an-index
    assert_equal [1, 0, -1], ids[0, true].to_a
  end

  def test_add_frozen
    index = Faiss::IndexBinaryFlat.new(32)
    index.freeze
    assert_raises(FrozenError) do
      index.add([[1, 1, 2, 1]])
    end
  end

  def test_remove_ids_frozen
    index = Faiss::IndexBinaryFlat.new(32)
    index.freeze
    assert_raises(FrozenError) do
      index.remove_ids([])
    end
  end

  def test_search_frozen
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexBinaryFlat.new(32)
    index.add(objects)
    index.freeze
    distances, ids = index.search(objects, 3)

    assert_equal [[0, 5, 6], [0, 5, 9], [0, 6, 9]], distances.to_a
    assert_equal [[0, 1, 2], [1, 0, 2], [2, 0, 1]], ids.to_a
  end

  def test_reconstruct
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexBinaryFlat.new(32)
    index.add(objects)

    3.times do |i|
      assert_equal objects[i], index.reconstruct(i).to_a
    end
  end

  def test_reconstruct_n
    objects = [
      [1, 1, 2, 1],
      [5, 4, 6, 5],
      [1, 2, 1, 2]
    ]
    index = Faiss::IndexBinaryFlat.new(32)
    index.add(objects)

    assert_equal [objects[1], objects[2]], index.reconstruct_n(1, 2).to_a

    # https://github.com/ruby-numo/numo-narray/issues/248
    unless valgrind?
      assert_equal Numo::UInt8.new(0, 4), index.reconstruct_n(1, 0)
    end

    error = assert_raises(ArgumentError) do
      index.reconstruct_n(1, -1)
    end
    assert_equal "expected n to be non-negative", error.message

    error = assert_raises(IndexError) do
      index.reconstruct_n(1, 3)
    end
    assert_match "index out of range", error.message

    error = assert_raises(IndexError) do
      index.reconstruct_n(-1, 1)
    end
    assert_match "index out of range", error.message

    error = assert_raises(IndexError) do
      index.reconstruct_n(3, 1)
    end
    assert_match "index out of range", error.message
  end

  private

  def max_int
    @max_int ||= 2**31 - 1
  end
end
