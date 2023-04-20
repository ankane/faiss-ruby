require "mkmf-rice"
require "numo/narray"

# libomp changed to keg-only
# https://github.com/Homebrew/homebrew-core/issues/112107
if RbConfig::CONFIG["host_os"] =~ /darwin/i
  brew_prefix = RbConfig::CONFIG["host_cpu"] =~ /arm|aarch64/i ? "/opt/homebrew" : "/usr/local"
  find_library("omp", nil, "#{brew_prefix}/opt/libomp/lib")
  find_header("omp.h", "#{brew_prefix}/opt/libomp/include")
end

abort "BLAS not found" unless have_library("blas")
abort "LAPACK not found" unless have_library("lapack")
abort "OpenMP not found" unless have_library("omp") || have_library("gomp")

numo = File.join(Gem.loaded_specs["numo-narray"].require_path, "numo")
abort "Numo not found" unless find_header("numo/narray.h", numo)

# for https://bugs.ruby-lang.org/issues/19005
$LDFLAGS += " -Wl,-undefined,dynamic_lookup" if RbConfig::CONFIG["host_os"] =~ /darwin/i

# -march=native not supported with ARM Mac
default_optflags = RbConfig::CONFIG["host_os"] =~ /darwin/i && RbConfig::CONFIG["host_cpu"] =~ /arm|aarch64/i ? "" : "-march=native"
$CXXFLAGS << " -std=c++17 $(optflags) -DFINTEGER=int " << with_config("optflags", default_optflags)

ext = File.expand_path(".", __dir__)
vendor = File.expand_path("../../vendor/faiss", __dir__)

$srcs = Dir["{#{ext},#{vendor}/faiss,#{vendor}/faiss/{impl,invlists,utils}/**}/*.{cpp}"]
$objs = $srcs.map { |v| v.sub(/cpp\z/, "o") }
abort "Faiss not found" unless find_header("faiss/Index.h", vendor)
$VPATH << vendor

create_makefile("faiss/ext")
