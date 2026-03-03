require "mkmf-rice"
require "numo/narray"

cmake_args = []

# libomp changed to keg-only
# https://github.com/Homebrew/homebrew-core/issues/112107
if RbConfig::CONFIG["host_os"] =~ /darwin/i
  brew_prefix = RbConfig::CONFIG["host_cpu"] =~ /arm|aarch64/i ? "/opt/homebrew" : "/usr/local"
  find_library("omp", nil, "#{brew_prefix}/opt/libomp/lib")
  find_header("omp.h", "#{brew_prefix}/opt/libomp/include")
  cmake_args += ["-DOpenMP_CXX_FLAGS=-Xpreprocessor -fopenmp -I#{brew_prefix}/opt/libomp/include", "-DOpenMP_CXX_LIB_NAMES=omp", "-DOpenMP_omp_LIBRARY=#{brew_prefix}/opt/libomp/lib/libomp.dylib"]
end

# abort "BLAS not found" unless have_library("blas")
# abort "LAPACK not found" unless have_library("lapack")
# abort "OpenMP not found" unless have_library("omp") || have_library("gomp")

numo = File.join(Gem.loaded_specs["numo-narray-alt"].require_path, "numo")
abort "Numo not found" unless find_header("numo/narray.h", numo)
abort "Numo library not found" if Gem.win_platform? && !find_library("narray", nil, "#{numo}/narray")

# for https://bugs.ruby-lang.org/issues/19005
$LDFLAGS += " -Wl,-undefined,dynamic_lookup" if RbConfig::CONFIG["host_os"] =~ /darwin/i

$CXXFLAGS += " -std=c++17 $(optflags) -DFINTEGER=int"

# -march=native not supported with ARM Mac
default_optflags = RbConfig::CONFIG["host_os"] =~ /darwin/i && RbConfig::CONFIG["host_cpu"] =~ /arm|aarch64/i ? "" : " -march=native"
$CXXFLAGS += with_config("optflags", default_optflags)

apple_clang = RbConfig::CONFIG["CC_VERSION_MESSAGE"] =~ /apple clang/i
$CXXFLAGS += " -Xclang" if apple_clang
# $CXXFLAGS += " -fopenmp"

ext = File.expand_path(".", __dir__)
vendor = File.expand_path("../../vendor/faiss", __dir__)

system "cmake", "-S", vendor, "-B", "build", "-DFAISS_OPT_LEVEL=dd", "-DFAISS_ENABLE_GPU=OFF", "-DFAISS_ENABLE_PYTHON=OFF", "-DFAISS_ENABLE_MKL=OFF", "-DFAISS_ENABLE_EXTRAS=OFF", "-DBUILD_TESTING=OFF", "-DCMAKE_INSTALL_PREFIX=install", *cmake_args
system "cmake", "--build", "build"
system "cmake", "--install", "build"
abort "Faiss not found" unless find_header("faiss/Index.h", "install/include")
abort "Faiss not found" unless find_library("faiss", nil, "install/lib")

create_makefile("faiss/ext")
