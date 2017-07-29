
add_library(AST
  Node.cpp
  Symbol.cpp
  SymbolTable.cpp)

add_subdirectory(NullaryNode)
add_subdirectory(UnaryNode)
add_subdirectory(BinaryNode)
# add_subdirectory(TernaryNode)
