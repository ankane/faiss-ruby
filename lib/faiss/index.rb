module Faiss
  class Index
    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      _train(objects.shape[0], objects)
    end

    def add(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      _add(objects.shape[0], objects)
    end

    def search(objects, k)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      n = objects.shape[0]
      distances, labels = _search(n, objects, k)
      [Numo::SFloat.from_binary(distances).reshape(n, k), Numo::Int64.from_binary(labels).reshape(n, k)]
    end
  end
end
