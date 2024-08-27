cmake_minimum_required(VERSION 3.20)
project(vrg_demo CXX)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/aurora)
set(LIBS ${LIBS} aurora)

find_package(EnTT REQUIRED)
set(LIBS ${LIBS} EnTT::EnTT)

add_executable(${PROJECT_NAME} source/main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBS})