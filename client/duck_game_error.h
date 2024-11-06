#ifndef DuckGameError_H
#define DuckGameError_H

#include <cstdint>
#include <stdexcept>
#include <string>

class DuckGameError : public std::exception {
private:
  // cppcheck-suppress unusedStructMember
  const int32_t line_number;
  // cppcheck-suppress unusedStructMember
  const std::string base_message;

  // cppcheck-suppress unusedStructMember
  std::string final_message;
  // cppcheck-suppress unusedStructMember
  const std::string module;


public:


  DuckGameError(const std::string &base_message, const int32_t &line_number,
            const std::string &module);

  const char *what() const noexcept;
};

#endif // DuckGameError_H