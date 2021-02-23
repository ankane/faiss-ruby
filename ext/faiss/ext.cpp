#include <rice/Module.hpp>

void init_index(Rice::Module& m);
void init_index_binary(Rice::Module& m);
void init_kmeans(Rice::Module& m);
void init_pca_matrix(Rice::Module& m);
void init_product_quantizer(Rice::Module& m);

extern "C"
void Init_ext()
{
  Rice::Module rb_mFaiss = Rice::define_module("Faiss");

  init_index(rb_mFaiss);
  init_index_binary(rb_mFaiss);
  init_kmeans(rb_mFaiss);
  init_pca_matrix(rb_mFaiss);
  init_product_quantizer(rb_mFaiss);
}
