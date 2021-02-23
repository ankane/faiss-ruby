#include <faiss/index_factory.h>

#include <rice/Module.hpp>

void init_index(Rice::Module& m);
void init_index_binary(Rice::Module& m);
void init_kmeans(Rice::Module& m);
void init_pca_matrix(Rice::Module& m);
void init_product_quantizer(Rice::Module& m);

extern "C"
void Init_ext()
{
  Rice::Module rb_mFaiss = Rice::define_module("Faiss")
    .define_singleton_method(
      "index_binary_factory",
      *[](int d, const char *description) {
        return faiss::index_binary_factory(d, description);
      });

  init_index(rb_mFaiss);
  init_index_binary(rb_mFaiss);
  init_kmeans(rb_mFaiss);
  init_pca_matrix(rb_mFaiss);
  init_product_quantizer(rb_mFaiss);
}
