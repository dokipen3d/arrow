project(fluidSim)
cmake_minimum_required(VERSION 2.8)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")

set(CMAKE_C_COMPILER "/usr/local/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

#set up linux search paths
set(LIBRARY_SEARCH_PATH
  /usr/local
  /opt/local
  /usr
  /opt
)


set(HEADER_SEARCH_PATH
  /usr/local
  /opt/local
  /usr
  /opt
)



find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h ${HEADER_SEARCH_PATH})
find_library(GLFW_LIBRARIES glfw3 ${LIBRARY_SEARCH_PATH})


find_path(FREETYPE_INCLUDE_DIR freetype2/ftbuild.h ${HEADER_SEARCH_PATH})
find_library(FREETYPE_LIBRARIES freetype ${LIBRARY_SEARCH_PATH})


#set(CMAKE_C_COMPILER /Users/dokipen/code/llvm/build/Release/bin/clang2)
#set(CMAKE_CXX_COMPILER /Users/dokipen/code/llvm/build/Release/bin/clang2++)

#include_directories(/Users/dokipen/code/llvm/build/include/c++/v1)
#include_directories(/Users/dokipen/code/openmp/runtime/build)

include_directories(/usr/local/include/freetype2)


#-lstdc++ -stdlib=libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}   -std=c++1y -pedantic  -O1")

aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})



find_package(OpenGL)
link_directories(/usr/local/lib/)

#find_library(GLFW3)
#MESSAGE(STATUS "Found GLFW3 in: ${GLFW3}")


target_link_libraries(${PROJECT_NAME} GLEW ${FREETYPE_LIBRARIES} ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} )
