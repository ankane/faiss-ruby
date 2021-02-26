require_relative "test_helper"

class KmeansTest < Minitest::Test
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
end
