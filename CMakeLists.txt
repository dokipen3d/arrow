project(arrow)
cmake_minimum_required(VERSION 3.8)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE} ${CMAKE_BINARY_DIR}${CMAKE_BUILD_TYPE})

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.frag
#        DESTINATION ${CMAKE_BUILD_TYPE})

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.vert
#        DESTINATION ${CMAKE_BUILD_TYPE})
        
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Oxygen-Regular.ttf
#        DESTINATION ${CMAKE_BUILD_TYPE})


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
target_include_directories(${PROJECT_NAME}  PRIVATE ${FREETYPE_INCLUDE_DIRS})


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-everything -Wno-error")




target_link_libraries(${PROJECT_NAME} 
                      PRIVATE 
                      ${FREETYPE_LIBRARIES} 
                      glfw
                      GLEW::GLEW 
                      ${OPENGL_LIBRARIES})
