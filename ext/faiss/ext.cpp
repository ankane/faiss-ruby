#include <faiss/IndexFlat.h>
#include <faiss/IndexHNSW.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/IndexLSH.h>
#include <faiss/IndexScalarQuantizer.h>
#include <faiss/IndexPQ.h>
#include <faiss/IndexIVFPQ.h>
#include <faiss/IndexIVFPQR.h>

#include <faiss/IndexBinaryFlat.h>
#include <faiss/IndexBinaryIVF.h>
#include <faiss/index_factory.h>

#include <rice/Class.hpp>
#include <rice/Constructor.hpp>
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

  Rice::define_class_under<faiss::IndexFlatL2, faiss::Index>(rb_mFaiss, "IndexFlatL2")
    .define_constructor(Rice::Constructor<faiss::IndexFlatL2, int64_t>());

  Rice::define_class_under<faiss::IndexFlatIP, faiss::Index>(rb_mFaiss, "IndexFlatIP")
    .define_constructor(Rice::Constructor<faiss::IndexFlatIP, int64_t>());

  Rice::define_class_under<faiss::IndexHNSWFlat, faiss::Index>(rb_mFaiss, "IndexHNSWFlat")
    .define_constructor(Rice::Constructor<faiss::IndexHNSWFlat, int, int>());

  Rice::define_class_under<faiss::IndexIVFFlat, faiss::Index>(rb_mFaiss, "IndexIVFFlat")
    .define_constructor(Rice::Constructor<faiss::IndexIVFFlat, faiss::Index*, size_t, size_t>());

  Rice::define_class_under<faiss::IndexLSH, faiss::Index>(rb_mFaiss, "IndexLSH")
    .define_constructor(Rice::Constructor<faiss::IndexLSH, int64_t, int>());

  Rice::define_class_under<faiss::IndexScalarQuantizer, faiss::Index>(rb_mFaiss, "IndexScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexScalarQuantizer, int, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexPQ, faiss::Index>(rb_mFaiss, "IndexPQ")
    .define_constructor(Rice::Constructor<faiss::IndexPQ, int, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFScalarQuantizer, faiss::Index>(rb_mFaiss, "IndexIVFScalarQuantizer")
    .define_constructor(Rice::Constructor<faiss::IndexIVFScalarQuantizer, faiss::Index*, size_t, size_t, faiss::ScalarQuantizer::QuantizerType>());

  Rice::define_class_under<faiss::IndexIVFPQ, faiss::Index>(rb_mFaiss, "IndexIVFPQ")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQ, faiss::Index*, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::IndexIVFPQR, faiss::Index>(rb_mFaiss, "IndexIVFPQR")
    .define_constructor(Rice::Constructor<faiss::IndexIVFPQR, faiss::Index*, size_t, size_t, size_t, size_t, size_t, size_t>());

  Rice::define_class_under<faiss::IndexBinaryFlat, faiss::IndexBinary>(rb_mFaiss, "IndexBinaryFlat")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryFlat, int64_t>());

  Rice::define_class_under<faiss::IndexBinaryIVF, faiss::IndexBinary>(rb_mFaiss, "IndexBinaryIVF")
    .define_constructor(Rice::Constructor<faiss::IndexBinaryIVF, faiss::IndexBinary*, size_t, size_t>());
}
