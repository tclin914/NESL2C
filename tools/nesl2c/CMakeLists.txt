
set(USEDLIBS
  LexerParser Node NullaryNode BinaryNode)

add_executable(NESL2C
  Main.cpp)

add_dependencies(NESL2C ${USEDLIBS})

target_link_libraries(NESL2C ${USEDLIBS})

install(TARGETS NESL2C RUNTIME DESTINATION bin)
