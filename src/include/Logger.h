#include <iostream>
class Logger {
 public:
  template<typename ... Args>
    static std::string formatMessage(const std::string& format,
		Args ... args)
    {
      // when the size is 0, the routine returns the size of the formated string
      size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'

      std::unique_ptr<char[]> buf(new char[size]);
      snprintf(buf.get(), size, format.c_str(), args ...);
      return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};
