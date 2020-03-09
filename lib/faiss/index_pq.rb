module Faiss
  class IndexPQ < Index
    def initialize(d, nbits)
      super()
      Utils.ffi(:faiss_IndexLSH_new, @handle.ref, d, nbits)
    end
  end
end
