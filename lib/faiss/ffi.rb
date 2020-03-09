module Faiss
  module FFI
    extend Fiddle::Importer

    libs = Array(Faiss.ffi_lib).dup
    begin
      dlload Fiddle.dlopen(libs.shift)
    rescue Fiddle::DLError => e
      retry if libs.any?
      raise e
    end

    typealias "idx_t", "int64_t"
    typealias "FaissMetricType", "int"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/Clustering_c.h
    extern "size_t faiss_Clustering_d(const Faiss Clustering *)"
    extern "size_t faiss_Clustering_k(const Faiss Clustering *)"
    extern "void faiss_Clustering_centroids(FaissClustering* clustering, float** centroids, size_t* size)"
    extern "int faiss_Clustering_new(FaissClustering** p_clustering, int d, int k)"
    extern "int faiss_Clustering_train(FaissClustering* clustering, idx_t n, const float* x, FaissIndex* index)"
    extern "void faiss_Clustering_free(FaissClustering* clustering)"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/IndexFlat_c.h
    extern "int faiss_IndexFlatL2_new_with(FaissIndexFlatL2** p_index, idx_t d)"
    extern "int faiss_IndexFlatIP_new_with(FaissIndexFlatIP** p_index, idx_t d)"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/IndexIVFFlat_c.h
    extern "int faiss_IndexIVFFlat_new_with(FaissIndexIVFFlat** p_index, FaissIndex* quantizer, size_t d, size_t nlist)"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/IndexLSH_c.h
    extern "int faiss_IndexLSH_new(FaissIndexLSH** p_index, idx_t d, int nbits)"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/Index_c.h
    extern "int faiss_Index_d(const Faiss Index *)"
    extern "int faiss_Index_is_trained(const Faiss Index *)"
    extern "idx_t faiss_Index_ntotal(const Faiss Index *)"
    extern "int faiss_Index_train(FaissIndex* index, idx_t n, const float* x)"
    extern "int faiss_Index_add(FaissIndex* index, idx_t n, const float* x)"
    extern "int faiss_Index_search(const FaissIndex* index, idx_t n, const float* x, idx_t k, float* distances, idx_t* labels)"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/error_c.h
    extern "const char* faiss_get_last_error()"

    # https://github.com/facebookresearch/faiss/blob/master/c_api/index_factory_c.h
    extern "int faiss_index_factory(FaissIndex** p_index, int d, const char* description, FaissMetricType metric)"
  end
end
