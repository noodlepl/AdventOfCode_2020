add_executable(Task4 main.cpp Passport.cpp Passport.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/input.txt ${CMAKE_BINARY_DIR}/Task4/input.txt COPYONLY)