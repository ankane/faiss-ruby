# dependencies
require "numo/narray"

# stdlib
require "fiddle/import"

# base classes
require "faiss/index"
require "faiss/index_binary"

# modules
require "faiss/index_binary_flat"
require "faiss/index_binary_ivf"
require "faiss/index_flat_l2"
require "faiss/index_flat_ip"
require "faiss/index_ivf_flat"
require "faiss/index_ivfpq"
require "faiss/index_ivfpqr"
require "faiss/index_lsh"
require "faiss/index_pq"
require "faiss/kmeans"
require "faiss/pca_matrix"
require "faiss/product_quantizer"
require "faiss/utils"
require "faiss/version"

module Faiss
  class Error < StandardError; end

  class << self
    attr_accessor :ffi_lib
  end
  lib_name =
    if Gem.win_platform?
      "faiss_c.dll"
    elsif RbConfig::CONFIG["host_os"] =~ /darwin/i
      "libfaiss_c.dylib"
    else
      "libfaiss_c.so"
    end
  vendor_lib = File.expand_path("../vendor/#{lib_name}", __dir__)
  self.ffi_lib = [vendor_lib]

  # friendlier error message
  autoload :FFI, "faiss/ffi"

  def self.index_binary_factory(d, description)
    index = Faiss::Index.new
    Utils.ffi(:faiss_index_factory, index.to_ptr.ref, d, description, 0)
    index
  end
end
