require "mkmf-rice"
require "numo/narray"

abort "BLAS not found" unless have_library("blas")
abort "LAPACK not found" unless have_library("lapack")
abort "OpenMP not found" unless have_library("omp") || have_library("gomp")

numo = $LOAD_PATH.find { |v| File.exist?("#{v}/numo/numo/narray.h") }
abort "Numo not found" unless numo && find_header("numo/narray.h", "#{numo}/numo")

$CXXFLAGS << " -std=c++17 $(optflags) -DFINTEGER=int " << with_config("optflags", "-march=native")

ext = File.expand_path(".", __dir__)
vendor = File.expand_path("../../vendor/faiss", __dir__)

$srcs = Dir["{#{ext},#{vendor}/faiss,#{vendor}/faiss/{impl,invlists,utils}}/*.{cpp}"]
$objs = $srcs.map { |v| v.sub(/cpp\z/, "o") }
abort "Faiss not found" unless find_header("faiss/Index.h", vendor)
$VPATH << vendor

create_makefile("faiss/ext")
