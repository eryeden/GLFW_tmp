#For OpenGL testing

cmake_minimum_required(VERSION 3.5)
set(BN "test1")

set(SOURCE_FILES main.cpp)
add_executable(${BN} ${SOURCE_FILES})

target_link_libraries(${BN} ${GL_LIBS})