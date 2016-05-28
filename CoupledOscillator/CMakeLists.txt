#For OpenGL testing

cmake_minimum_required(VERSION 3.5)
set(BN "CoupledOscillator")

add_definitions(-DSHADER_PREFIX="${PROJECT_SOURCE_DIR}/GLSL/")
add_definitions(-DFONTS_DIR_PREFIX="${PROJECT_SOURCE_DIR}/Fonts/")
add_definitions(-DIMAGE_DIR="${PROJECT_SOURCE_DIR}/MovieDir/")

set(SOURCE_FILES main.cpp ${COMMON_SRCS} World.cpp GraphicsBase.cpp)
add_executable(${BN} ${SOURCE_FILES})

target_link_libraries(${BN} ${GL_LIBS} ${FREETYPE_LIB})



