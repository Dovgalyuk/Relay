cmake_minimum_required(VERSION 3.5)
find_package(BISON)
find_package(FLEX)
BISON_TARGET(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
			 DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h)
FLEX_TARGET(Scanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp
			DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(compiler main.cpp
			   ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})
