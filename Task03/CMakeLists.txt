add_executable(Task03 main.cpp AreaMap.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/Task03/input.txt COPYONLY)