module Faiss
  class ProductQuantizer
    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      _train(objects.shape[0], objects.to_binary)
    end

    def compute_codes(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      n = objects.shape[0]
      res = _compute_codes(n, objects.to_binary)
      Numo::UInt8.from_binary(res).reshape(n, m)
    end

    def decode(objects)
      objects = Numo::UInt8.cast(objects) unless objects.is_a?(Numo::UInt8)
      n = objects.shape[0]
      res = _decode(n, objects.to_binary)
      Numo::SFloat.from_binary(res).reshape(n, d)
    end
  end
end
