module Faiss
  class IndexBinaryFlat < IndexBinary
    def initialize(d)
      super()
      Utils.ffi(:faiss_IndexFlatIP_new_with, @handle.ref, d)
    end
  end
end
