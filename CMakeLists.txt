project(arrow)
cmake_minimum_required(VERSION 2.8)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")

find_package(freetype CONFIG REQUIRED)
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

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.vert
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

        
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Oxygen-Regular.ttf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



target_link_libraries(${PROJECT_NAME} 
                      PRIVATE 
                      freetype 
                      glfw
                      GLEW::GLEW 
                      ${OPENGL_LIBRARIES})
