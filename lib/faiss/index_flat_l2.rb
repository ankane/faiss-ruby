module Faiss
  class IndexFlatL2 < Index
    def initialize(d)
      super()
      Utils.ffi(:faiss_IndexFlatL2_new_with, @handle.ref, d)
    end
  end
end
