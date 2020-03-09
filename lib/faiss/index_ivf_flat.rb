module Faiss
  class IndexIVFFlat < Index
    def initialize(quantizer, d, nlist)
      super()
      Utils.ffi(:faiss_IndexIVFFlat_new_with, @handle.ref, quantizer, d, nlist)
    end
  end
end
