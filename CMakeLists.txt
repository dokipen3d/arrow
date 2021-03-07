project(arrow)
cmake_minimum_required(VERSION 2.8)

message("system: ${CMAKE_SYSTEM_PREFIX_PATH}")
message(STATUS " build type is ${CMAKE_BUILD_TYPE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
message(STATUS " dest is ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}}")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.frag
        DESTINATION ${CMAKE_BUILD_TYPE})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/textShader.vert
        DESTINATION ${CMAKE_BUILD_TYPE})
        
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/Oxygen-Regular.ttf
        DESTINATION ${CMAKE_BUILD_TYPE})

##add the installed shared libraries directory to your PATH environment variable
#something like 'vcpkg/installed/x64-windows/bin/'
find_package(freetype  REQUIRED ) # should find freetype-config.cmake if available
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} )
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})


#include_directories(/usr/local/include/freetype2)

aux_source_directory(src/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)




target_link_libraries(${PROJECT_NAME} 
                      PRIVATE 
                      ${FREETYPE_LIBRARIES}
                      glfw
                      GLEW::GLEW
                      ${GLM_LIBRARIES}
                      ${OPENGL_LIBRARIES})
