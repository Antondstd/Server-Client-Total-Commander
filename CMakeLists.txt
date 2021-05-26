cmake_minimum_required(VERSION 3.17)
project(Lab3 C)

set(CMAKE_C_STANDARD 99)

add_executable(Lab3 main.c Console.h Console.c ApiFilesAndDir.c ApiFilesAndDir.h ClientInput.c ClientInput.h serverApiFiles.c serverApiFiles.h)