#For OpenGL testing

cmake_minimum_required(VERSION 3.5)
set(BN "World")

add_definitions(-DSHADER_PREFIX="${PROJECT_SOURCE_DIR}/GLSL/")

set(SOURCE_FILES main.cpp ${COMMON_SRCS} World.cpp)
add_executable(${BN} ${SOURCE_FILES})

target_link_libraries(${BN} ${GL_LIBS})



