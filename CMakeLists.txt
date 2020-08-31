project(arrow)
cmake_minimum_required(VERSION 2.8)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")

find_package(Freetype REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories(${GLFW_INCLUDE_DIRS})


#include_directories(/usr/local/include/freetype2)

aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)


target_link_libraries(${PROJECT_NAME} 
                      PRIVATE 
                      Freetype 
                      glfw
                      GLEW::GLEW 
                      ${OPENGL_LIBRARIES})
