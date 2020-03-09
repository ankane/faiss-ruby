module Faiss
  module Utils
    def self.ffi(method, *args)
      result = FFI.send(method, *args)
      raise Error, FFI.faiss_get_last_error.to_s if result != 0
      nil
    end

    # TODO get pointer directly from Numo
    def self.numo_ptr(objects)
      Fiddle::Pointer[objects.to_binary]
    end

    def self.numo_object(cls, ptr, shape)
      cls.from_binary(ptr[0, ptr.size]).reshape(*shape)
    end
  end
end
