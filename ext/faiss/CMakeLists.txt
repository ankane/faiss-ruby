cmake_minimum_required (VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# The Ruby extension is a shared library
project("faiss_ruby" LANGUAGES C CXX)
add_library (${CMAKE_PROJECT_NAME} SHARED)

# Find Ruby
find_package(Ruby REQUIRED)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${Ruby_INCLUDE_DIR} ${Ruby_CONFIG_INCLUDE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} ${Ruby_LIBRARY})

# Find FAISS
include(CMakeFindDependencyMacro)
find_package(faiss CONFIG REQUIRED)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${faiss_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} faiss)

# Get narray include path and lib dir
execute_process(COMMAND ${Ruby_EXECUTABLE} -r "numo/narray" -e "puts Gem.loaded_specs['numo-narray'].extension_dir"
                RESULT_VARIABLE status
                OUTPUT_VARIABLE narray_extension_dir
                COMMAND_ERROR_IS_FATAL ANY)
cmake_path(APPEND narray_include_dir "${narray_extension_dir}" "numo")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "narray_include_dir")
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC "narray_include_dir")

# Get Rice headers
execute_process(COMMAND ${Ruby_EXECUTABLE} -r "rice" -e "puts Gem.loaded_specs['rice'].full_gem_path"
                RESULT_VARIABLE status
                OUTPUT_VARIABLE rice_dir
                COMMAND_ERROR_IS_FATAL ANY)
cmake_path(APPEND rice_include_dir "${rice_dir}" "include")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "rice_include_dir")

# Define project properties
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
                      PREFIX ""
                      SUFFIX ".dll"
                      OUTPUT_NAME "faiss_ruby"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../lib" # Windows
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../../lib" # Not Windows
                      )

# Add source files
target_sources(${CMAKE_PROJECT_NAME} PUBLIC
               "faiss_ruby.def"
               "faiss_ruby.cpp"
               "index.cpp"
               "index_binary.cpp"
               "kmeans.cpp"
               "pca_matrix.cpp"
               "product_quantizer.cpp"
               "utils.cpp")

