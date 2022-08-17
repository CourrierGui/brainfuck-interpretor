#include <string>
#include <fstream>
#include <iostream>

#include "bf-interpretor.hpp"

int main(int argc, char** argv) {
  std::string input_file;

  if (argc == 1 || argc > 2) {
      std::cerr << "Usage: " << argv[0] << " <filename>\n";
      return 1;
  }

  input_file = argv[1];

  try {
    BFInterpretor bfi{input_file};
    bfi.start();
  } catch(const InvalidSyntax& e) {
    std::cerr
      << "ParserError: InvalidSyntax: line "
      << e.line
      << ", col "
      << e.col
      << " got "
      << e.c
      << ".\n";
    return 1;
  }
  return 0;
}
