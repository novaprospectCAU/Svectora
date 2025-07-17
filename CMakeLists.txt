cmake_minimum_required(VERSION 3.16)

project(Svectora)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# compile_commands.json 생성
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 소스 파일
set(SOURCES
    src/main.cpp
)

# 컴파일 옵션 수정된 부분
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# 실행 파일
add_executable(app ${SOURCES})

# SDL2 찾기
find_package(SDL2 REQUIRED)

# OpenGL 라이브러리 찾기
find_library(OPENGL_LIBRARY OpenGL)

# include 경로
target_include_directories(app PRIVATE ${SDL2_INCLUDE_DIRS})

# 링크
target_link_libraries(app PRIVATE ${SDL2_LIBRARIES} ${OPENGL_LIBRARY})
