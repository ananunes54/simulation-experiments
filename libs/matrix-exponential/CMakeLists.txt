cmake_minimum_required(VERSION 3.16)
project(matrix-exponential)

add_library(expm STATIC EXCLUDE_FROM_ALL src/matrix_exponential.c src/r8lib.c src/c8lib.c)
target_include_directories(expm PUBLIC include)

