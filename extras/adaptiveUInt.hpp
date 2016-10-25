#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

template <class T> constexpr bool is_in_range(std::uintmax_t x) {
  return (x >= std::numeric_limits<T>::min()) &&
         (x <= std::numeric_limits<T>::max());
}

template <std::uintmax_t x>
using AdaptiveUInt = typename std::conditional<
    is_in_range<std::uint_least8_t>(x), std::uint_least8_t,
    typename std::conditional<
        is_in_range<std::uint_least16_t>(x), std::uint_least16_t,
        typename std::conditional<
            is_in_range<std::uint_least32_t>(x), std::uint_least32_t,
            typename std::enable_if<is_in_range<std::uint_least64_t>(x),
                                    std::uint_least64_t>::type>::type>::type>::
    type;

/*==============================================================================
  Read the header of a file
==============================================================================*/
std::vector<std::string> readHeader(std::ifstream &file) {
  std::string str;
  std::vector<std::string> line;
  size_t pos = std::string::npos;

  getline(file, str); // to read the first HEADER LINE

  getline(file, str);
  while (str.compare("/HEADER ") != 0) {
    pos = str.find_first_of(" ");
    line.push_back(str.substr(0, pos));
    line.push_back(str.substr(pos + 1, str.size() - pos - 1));
    getline(file, str);
  }
  return line;
}

/*==============================================================================
  Get the (expected) size of input
==============================================================================*/
std::uintmax_t inputSize(std::string filename) {

  std::ifstream file;
  file.open(filename, std::ifstream::in);
  std::vector<std::string> parameters = readHeader(file);
  file.close();

  std::uintmax_t n = 0;

  for (unsigned int i = 0; i < parameters.size(); i = i + 2) {
    if (parameters[i].compare("n") == 0)
      n = stoi(parameters[i + 1]);
  }

  return n;
}
