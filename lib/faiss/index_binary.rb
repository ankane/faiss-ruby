module Faiss
  class IndexBinary
    def train(objects)
      objects = Numo::UInt8.cast(objects) unless objects.is_a?(Numo::UInt8)
      _train(objects.shape[0], objects)
    end

    def add(objects)
      objects = Numo::UInt8.cast(objects) unless objects.is_a?(Numo::UInt8)
      _add(objects.shape[0], objects)
    end

    def search(objects, k)
      objects = Numo::UInt8.cast(objects) unless objects.is_a?(Numo::UInt8)
      n = objects.shape[0]
      distances, labels = _search(n, objects, k)
      [Numo::UInt32.from_binary(distances).reshape(n, k), Numo::Int64.from_binary(labels).reshape(n, k)]
    end
  end
end
