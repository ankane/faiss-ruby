#include "utils.h"

void init_index(Rice::Module& m);
void init_index_binary(Rice::Module& m);
void init_kmeans(Rice::Module& m);
void init_pca_matrix(Rice::Module& m);
void init_product_quantizer(Rice::Module& m);

extern "C"
void Init_ext()
{
  auto m = Rice::define_module("Faiss");

  init_index(m);
  init_index_binary(m);
  init_kmeans(m);
  init_pca_matrix(m);
  init_product_quantizer(m);
}
