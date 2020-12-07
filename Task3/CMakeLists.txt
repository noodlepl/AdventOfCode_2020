add_executable(Task3 main.cpp AreaMap.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/Task3/input.txt COPYONLY)