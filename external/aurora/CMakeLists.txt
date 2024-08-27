project(aurora CXX)

add_library(aurora INTERFACE) 
target_include_directories(aurora INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)