require_relative "test_helper"

class PCAMatrixTest < Minitest::Test
  def test_pca_matrix
    mt = Numo::SFloat.new(1000, 40).rand
    mat = Faiss::PCAMatrix.new(40, 10)
    mat.train(mt)
    tr = mat.apply(mt)
    assert_operator (tr ** 2).sum(0).to_a.last, :<, 100
  end
end
