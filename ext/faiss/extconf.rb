require "mkmf-rice"

abort "Missing faiss" unless have_library("faiss")

$CXXFLAGS << " -std=c++11"

create_makefile("faiss/ext")
