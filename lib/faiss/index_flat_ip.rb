module Faiss
  class IndexFlatIP < Index
    def initialize(d)
      super()
      Utils.ffi(:faiss_IndexFlatIP_new_with, @handle.ref, d)
    end
  end
end
