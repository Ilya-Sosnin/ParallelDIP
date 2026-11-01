cmake_minimum_required(VERSION 3.30)

project(ParallelDIP
        VERSION 0.0.1
        DESCRIPTION "Parallel Digital Image Processing"
        LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(
    ParallelDIP
    src/main.c
    src/bmp_reader.c
    src/bmp_writer.c
)

target_include_directories(
    ParallelDIP PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)