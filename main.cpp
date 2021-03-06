#include <string>
#include <fstream>
#include <iostream>

#include "bf-interpretor.hpp"

int main(int argc, char** argv) {
  std::string input_file;

  if (argc > 1) {
    input_file = argv[1];
  } else {
    std::ifstream help("help.txt");

    if (help.is_open()) {
      std::string line;

      while (getline(help, line)) {
        std::cerr << line << '\n';
      }
      help.close();
    }
    else std::cerr << "Unable to find help.\n";
  }

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
