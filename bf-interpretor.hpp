#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct InvalidSyntax {
  char c;
};

class Parser {
  private:
    std::vector<char> m_inst;

  public:
    Parser(const std::string& input_file) {
      char c;

      std::ifstream bf_file(input_file);
      bool comments = false;
      while (bf_file >> std::noskipws >> c) {
        if (c == '#') {
          comments = true;
          continue;
        }
        if (comments && c == '\n') {
          comments = false;
          continue;
        } else if (comments) continue;

        if (c == '\n' || c == ' ') continue;
        m_inst.push_back(c);
      }
    }

    std::vector<char> get_instructions() {
      return m_inst;
    }
};

class BFInterpretor {
  private:
    using instruction_type = std::vector<char>;
    using buffer_type = std::array<int, 30000>;

    buffer_type m_buffer;
    buffer_type::iterator m_ptr;
    bool log;

    instruction_type m_inst;
    instruction_type::iterator m_inst_ptr;

  public:
    BFInterpretor(const std::string& input_file)
      : m_buffer({0}),
        m_ptr(m_buffer.begin()),
        log(false) {

      Parser parser(input_file);
      m_inst = parser.get_instructions();
      m_inst_ptr = m_inst.begin();
    }

    void start() {
      while (m_inst_ptr != m_inst.end()) {
        /* std::clog << "c0: " << m_buffer[0] << " c1: " << m_buffer[1] << '\n'; */
        /* std::clog << *m_inst_ptr << '\n'; */
        try {
          step(*m_inst_ptr);
        } catch(const InvalidSyntax& e) {
          std::cerr << "\n\nInvalid syntax at instruction "
            << std::distance(m_inst.begin(), m_inst_ptr)
            << " got "
            << *m_inst_ptr
            << '\n';
          return;
        }
      }
      if (log) std::cout << '\n';
    }

    void step(const char c) {
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
          if (!log) log = true;
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
          throw InvalidSyntax{.c = c};
          break;
      }
    }

    void next_match() {
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

    void previous_match() {
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
