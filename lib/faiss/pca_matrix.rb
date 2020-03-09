module Faiss
  class PCAMatrix
    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      _train(objects.shape[0], objects)
    end

    def apply(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      n = objects.shape[0]
      res = _apply(n, objects)
      Numo::SFloat.from_binary(res).reshape(n, d_out)
    end
  end
end
