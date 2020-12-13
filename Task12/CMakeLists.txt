add_executable(Task12 main.cpp PositionPart1.h PositionPart2.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/Task12/input.txt COPYONLY)