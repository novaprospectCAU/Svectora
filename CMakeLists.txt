cmake_minimum_required(VERSION 3.16)

project(Svectora)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 소스 파일
set(SOURCES
    src/resources/vertices.cpp
    src/utils/readFile.cpp
    src/main.cpp
    includes/glad/glad.c
)

add_executable(app ${SOURCES})

# SDL2
find_package(SDL2 REQUIRED)
target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(app PRIVATE ${SDL2_LIBRARIES})

# OpenGL (macOS에서는 프레임워크 형태로 링크됨)
find_library(OPENGL_LIBRARY OpenGL REQUIRED)
target_link_libraries(app PRIVATE ${OPENGL_LIBRARY})

# glad
target_include_directories(app PRIVATE includes)

# glm
find_package(glm CONFIG REQUIRED)
target_link_libraries(app PRIVATE glm::glm)

# 컴파일 옵션 (공통)
target_compile_options(app PRIVATE -Wall -Wextra -Wpedantic -Werror)

# 핵심! macOS에서 deprecation warning 제거
if (APPLE)
    target_compile_definitions(app PRIVATE GL_SILENCE_DEPRECATION=1)
    target_compile_options(app PRIVATE -Wno-deprecated-declarations)
endif()
