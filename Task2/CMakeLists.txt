add_executable(Task2 main.cpp Password.cpp Password.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input ${CMAKE_BINARY_DIR}/Task2/input COPYONLY)