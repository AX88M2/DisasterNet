cmake_minimum_required(VERSION 4.2)
project(DisasterNet)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdparty/enet)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp)

add_library(disnet SHARED ${SOURCES})

target_include_directories(disnet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(disnet PRIVATE enet_static)
