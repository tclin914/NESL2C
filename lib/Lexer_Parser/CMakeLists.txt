
find_package(BISON)
find_package(FLEX)

BISON_TARGET(Parser nesl.y ${CMAKE_CURRENT_BINARY_DIR}/nesl.tab.cc)
FLEX_TARGET(Lexer nesl.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cc)

add_library(LexerParser
  nesl.tab.cc lex.yy.cc)
