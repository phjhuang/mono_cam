///////////////////////////////////////////////////////////////////
// 类型转化函数
///////////////////////////////////////////////////////////////////
#ifndef _CONVERT_HPP_
#define _CONVERT_HPP_
//returning address of local variable or temporary
#pragma warning(disable: 4172)

#define IS_DIGIT(c) (c>='0'&&c<='9')
#define IS_LOWERCASE(c) (c>='a'&&c<='z')
#define IS_UPPERCASE(c) (c>='A'&&c<='Z')
#define IS_LETTERS(c) ((IS_LOWERCASE(c))||(IS_UPPERCASE(c)))
#define _sprintf  sprintf

#include <string>
#include <strstream>
#include <iomanip>
//#include "base.hpp"
//#include "macro_helper.hpp"
///////////////////////////////////////////////////////////////////
namespace detail {
struct lexical_stream {
  std::strstream  stream;
  template <typename T>
  bool operator<<(const T& in) {
    return !(stream << in).fail();
  }

  template <typename T>
  bool operator>>(T& out) {
    return !(stream >> out).fail();
  }
};
}; // namespace detail

template <typename R, typename T>
R lexical_cast(const T& in) {
  detail::lexical_stream stream;
  R out;
  stream << in;
  stream >> out;
  return out;
}

#define TEMPLATE_INLINE1(T)  template<typename T > inline

TEMPLATE_INLINE1(T)
std::string num2str(T x, size_t n = 6) {
  std::setprecision(n);
  return lexical_cast<std::string>(x);
}

inline
std::string int2str(size_t x, size_t n = 0) {
  std::string ret = lexical_cast<std::string>(x);
  int m = n - ret.length();
  if (m>0) {
    std::string ret0;
    ret0.resize(m);
    std::fill(ret0.begin(), ret0.end(), '0');
    return ret0+ret;
  }
  return ret;
}

TEMPLATE_INLINE1(T)
std::string num2str(T x, const char* fmt) {
  char buf[1024]  = { 0 };
  _snprintf(buf, 1024, fmt, x);
  return std::string(buf);
}

/////////////////////////////////////////////////////////////////////
// 函数名: _dtoa
// 功能: 将一个双精度型变量转换成字符串（保留三位有效的小数位数）
// 用法: void   _dtoa(double value, char *string)
// 参数: value: 双精度型变量，string: 字符串
// 返回: 无
/////////////////////////////////////////////////////////////////////
inline char* _dtoa(double v) {
  enum {
    SIZE = 64
  };
  static char str[SIZE];
  _sprintf(str, "%16.15f", v);
  for (size_t i = SIZE - 1; i && (str[i] == '\0' || str[i] == '0'); --i) {
    str[i] = 0;
  }
  return str;
}
///////////////////////////////////////////////////////////////////
#endif //_CONVERT_HPP_