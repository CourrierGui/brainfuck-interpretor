#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct InvalidSyntax {
  char c;
  int line, col;
};

class Parser {
  private:
    std::vector<char> m_inst;
    std::array<char, 8> values = {'>', '<', '-', '+', '.', ',', '[', ']'};

  public:
    Parser(const std::string& input_file) {
      char c;
      int line(1), col(1);

      std::ifstream bf_file(input_file);
      bool comments = false;
      while (bf_file >> std::noskipws >> c) {
        ++col;
        if (c == '\n') {
          col = 0;
          ++line;
        }
        if (c == '#') {
          comments = true;
          continue;
        }
        if (comments && c == '\n') {
          comments = false;
          continue;
        }
        else if (comments) continue;

        if (c == '\n' || c == ' ') continue;
        if (std::find(values.begin(), values.end(), c) != values.end())
          m_inst.push_back(c);
        else {
          throw InvalidSyntax{.c = c, .line = line, .col = col};
        }
      }
    }

    std::vector<char> get_instructions() {
      return m_inst;
    }
};

class BFInterpretor {
  private:
    using instruction_type = std::vector<char>;
    using buffer_type = std::array<char, 30000>;

    buffer_type m_buffer;
    buffer_type::iterator m_ptr;

    instruction_type m_inst;
    instruction_type::iterator m_inst_ptr;

  public:
    BFInterpretor(const std::string& input_file)
      : m_buffer({0}),
      m_ptr(m_buffer.begin()) {

        try {
          Parser parser(input_file);
          m_inst = parser.get_instructions();
          m_inst_ptr = m_inst.begin();
        } catch(const InvalidSyntax& e) {
          throw e;
        }
      }

    void start() noexcept {
      while (m_inst_ptr != m_inst.end()) {
        try {
          step(*m_inst_ptr);
        } catch(const InvalidSyntax& e) {
          std::cerr << "\n\nInvalid syntax at instruction "
            << std::distance(m_inst.begin(), m_inst_ptr)
            << " got "
            << e.c
            << '\n';
          return;
        }
      }
    }

    void step(const char c) noexcept {
      switch (c) {
        case '>':
          ++m_ptr;
          ++m_inst_ptr;
          break;
        case '<':
          --m_ptr;
          ++m_inst_ptr;
          break;
        case '+':
          ++(*m_ptr);
          ++m_inst_ptr;
          break;
        case '-':
          --(*m_ptr);
          ++m_inst_ptr;
          break;
        case '.':
          std::cout << *m_ptr;
          ++m_inst_ptr;
          break;
        case ',':
          std::cin >> *m_ptr;
          ++m_inst_ptr;
          break;
        case '[':
          if (!*m_ptr) {
            next_match();
          }
          else ++m_inst_ptr;
          break;
        case ']':
          if (*m_ptr) {
            previous_match();
          }
          else ++m_inst_ptr;
          break;
        default:
          break;
      }
    }

    void next_match() noexcept {
      int count = 1;
      while (true) {
        ++m_inst_ptr;
        if (*m_inst_ptr == ']') {
          --count;
          if (!count) {
            ++m_inst_ptr;
            break;
          }
        } else if (*m_inst_ptr == '[')
          ++count;
      }
    }

    void previous_match() noexcept {
      int count = 1;
      while (true) {
        --m_inst_ptr;
        if (*m_inst_ptr == ']') {
          ++count;
        } else if (*m_inst_ptr == '[')
          --count;
        if (!count) {
          ++m_inst_ptr;
          break;
        }
      }
    }
};
