#For OpenGL testing

cmake_minimum_required(VERSION 3.5)
set(BN "test2")

add_definitions(-DSHADER_PREFIX="${CMAKE_CURRENT_SOURCE_DIR}/")

set(SOURCE_FILES main.cpp ${COMMON_SRCS})
add_executable(${BN} ${SOURCE_FILES})

target_link_libraries(${BN} ${GL_LIBS})



