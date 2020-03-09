module Faiss
  class Kmeans
    attr_reader :index

    def initialize(d, k)
      @handle = Fiddle::Pointer.malloc(Fiddle::SIZEOF_VOIDP)
      Utils.ffi(:faiss_Clustering_new, @handle.ref, d, k)
      ObjectSpace.define_finalizer(self, self.class.finalize(@handle))
    end

    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      @index = IndexFlatL2.new(d)
      Utils.ffi(:faiss_Clustering_train, @handle, objects.shape[0], Utils.numo_ptr(objects), @index)
    end

    def d
      FFI.faiss_Clustering_d(@handle)
    end

    def k
      FFI.faiss_Clustering_k(@handle)
    end

    def centroids
      centroids = Fiddle::Pointer.malloc(k * d * Fiddle::SIZEOF_FLOAT)
      size = Fiddle::Pointer.malloc(Fiddle::SIZEOF_INT)
      FFI.faiss_Clustering_centroids(@handle, centroids.ref, size)
      Utils.numo_object(Numo::SFloat, centroids, [k, d])
    end

    def self.finalize(pointer)
      # must use proc instead of stabby lambda
      proc { FFI.faiss_Clustering_free(pointer) }
    end
  end
end
