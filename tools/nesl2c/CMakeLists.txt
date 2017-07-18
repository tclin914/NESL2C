
set(USED_LIBS
  LexerParser Node NullaryNode BinaryNode Visitor)

add_executable(NESL2C
  Main.cpp)

# add_dependencies(NESL2C ${USEDLIBS})

llvm_map_components_to_libnames(LLVM_LIBS core support)

target_link_libraries(NESL2C ${USED_LIBS})
target_link_libraries(NESL2C ${LLVM_LIBS})

install(TARGETS NESL2C RUNTIME DESTINATION bin)
