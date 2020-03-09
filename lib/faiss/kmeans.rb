module Faiss
  class Kmeans
    attr_reader :index

    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      @index = IndexFlatL2.new(d)
      _train(objects.shape[0], objects, @index)
    end

    def centroids
      Numo::SFloat.from_binary(_centroids).reshape(k, d)
    end
  end
end
