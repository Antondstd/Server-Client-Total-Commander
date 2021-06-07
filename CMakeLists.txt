cmake_minimum_required(VERSION 3.17)
project(Lab3 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -static -lws2_32")
link_libraries(ws2_32 wsock32)
add_executable(Lab3 main.c Console.h Console.c ApiFilesAndDir.c ApiFilesAndDir.h ClientInput.c ClientInput.h serverApiFiles.c serverApiFiles.h messages.c messages.h server.c server.h client.c client.h serverFiles.c serverFiles.h MyTypes.h)