require_relative "test_helper"

class ProductQuantizerTest < Minitest::Test
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
end
