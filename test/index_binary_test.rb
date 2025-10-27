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

  def test_add_frozen
    index = Faiss::IndexBinaryFlat.new(32)
    index.freeze
    assert_raises(FrozenError) do
      index.add([[1, 1, 2, 1]])
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

  private

  def max_int
    @max_int ||= 2**31 - 1
  end
end
