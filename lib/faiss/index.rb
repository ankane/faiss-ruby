module Faiss
  class Index
    def initialize
      @handle = Fiddle::Pointer.malloc(Fiddle::SIZEOF_VOIDP)
    end

    def train(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      Utils.ffi(:faiss_Index_train, @handle, objects.shape[0], Utils.numo_ptr(objects))
    end

    def add(objects)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      Utils.ffi(:faiss_Index_add, @handle, objects.shape[0], Utils.numo_ptr(objects))
    end

    def search(objects, k)
      objects = Numo::SFloat.cast(objects) unless objects.is_a?(Numo::SFloat)
      n = objects.shape[0]
      distances = Fiddle::Pointer.malloc(k * n * Fiddle::SIZEOF_FLOAT)
      labels = Fiddle::Pointer.malloc(k * n * Fiddle::SIZEOF_LONG_LONG)
      Utils.ffi(:faiss_Index_search, @handle, n, Utils.numo_ptr(objects), k, distances, labels)
      [Utils.numo_object(Numo::SFloat, distances, [n, k]), Utils.numo_object(Numo::Int64, labels, [n, k])]
    end

    def d
      FFI.faiss_Index_d(@handle)
    end

    def trained?
      FFI.faiss_Index_is_trained(@handle) != 0
    end

    def ntotal
      FFI.faiss_Index_ntotal(@handle)
    end

    def to_ptr
      @handle
    end
  end
end
