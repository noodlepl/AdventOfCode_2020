add_executable(Task04 main.cpp Passport.cpp Passport.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/Task04/input.txt COPYONLY)