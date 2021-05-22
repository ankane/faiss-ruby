module Faiss
  class Kmeans
    attr_reader :index

    def train(objects)
      @index = IndexFlatL2.new(d)
      _train(objects, @index)
    end
  end
end
