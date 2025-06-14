cmake_minimum_required(VERSION 3.10)
project(sfSnake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

# 指定 SFML 的 include 和 lib 路径
include_directories(${CMAKE_SOURCE_DIR}/3rd/sfml/include)
link_directories(${CMAKE_SOURCE_DIR}/3rd/sfml/lib)

# 源文件
file(GLOB SOURCES "src/*.cpp")

# 可执行文件
add_executable(sfSnake ${SOURCES})

# 链接SFML库
target_link_libraries(sfSnake
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# 复制资源文件到输出目录
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${EXECUTABLE_OUTPUT_PATH})

# 复制SFML DLL文件到输出目录
file(COPY ${CMAKE_SOURCE_DIR}/3rd/sfml/bin/ DESTINATION ${EXECUTABLE_OUTPUT_PATH})

if(MSVC)
    target_compile_options(sfSnake PRIVATE /utf-8)
endif()

if(WIN32)
    set_target_properties(sfSnake PROPERTIES WIN32_EXECUTABLE TRUE)
endif() 