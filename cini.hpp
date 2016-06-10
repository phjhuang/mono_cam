
#ifndef _CINI_HPP_
#define _CINI_HPP_

#include <stdlib.h>
#include <vector>
#include <string>
#include ".\lexical_cast.hpp"
#include <windows.h>

class cini {
public:
  enum {
    MAX = MAX_PATH
  };
  typedef cini self;
  typedef std::string A;
public:
  struct inibind {
    inibind()
      : content(0)
    {
    }

    template <typename B>
    inibind(const A& a, B& b)
      : content(new holder<B>(a, b))
    {
    }

    inibind(const inibind& other)
      : content(other.content ? other.content->clone() : 0)
    {
    }

    ~inibind() {
      delete content;
    }
  public: // modifiers

    inibind& swap(inibind& rhs) {
      std::swap(content, rhs.content);
      return *this;
    }

    inibind& operator=(const inibind& rhs) {
      inibind(rhs).swap(*this);
      return *this;
    }

    bool empty() const {
      return !content;
    }

    void save(self& ini) {
      content->save(ini);
    }
    void load(self& ini) {
      content->load(ini);
    }

    class placeholder {
    public: // structors    
      virtual ~placeholder() {
      }
    public: // queries
      virtual placeholder* clone() const = 0;
      virtual void save(self& ini) = 0;
      virtual void load(self& ini) = 0;
    };
    template <typename B>
    class holder : public placeholder {
    public: // structors
      holder(const A& a, B& b)
        : key(a)
        , val(b)
      {
      }
    public: // queries
      virtual placeholder* clone() const {
        return new holder(key, val);
      }
      virtual void save(self& ini) {
        ini.set(key, val);
      }
      virtual void load(self& ini) {
        ini.get(key, val);
      }
    public: // representation
      A key;
      B& val;
    };
    placeholder* content;
  };
public:
  std::string file_path;
  std::string appname;
  std::vector<inibind> vib;
public:
  cini(const char* fn = NULL) {
    if (fn)
      open(fn);
  }
  cini(const std::string fn)
    : file_path(fn)
  {
  }
  void open(const char* fn) {
    file_path = (fn);
	vib.clear();
  }
  //~cini() { for (int i=0; i<vib.size(); ++i) { delete (vib[i]); } }
private:
  void get_str(const char* kk, char* s) const {
    ::GetPrivateProfileString(appname.c_str(), kk, s, s, MAX,
      file_path.c_str());
  }

  void set_str(const char* kk, const char* s) const {
    ::WritePrivateProfileString(appname.c_str(), kk, s, file_path.c_str());
  }

  void get_int(const char* kk, int& i) const {
    i = GetPrivateProfileInt(appname.c_str(), kk, 1, file_path.c_str());
  }

  void set_int(const char* kk, const int& i) const {
    char r[10]; _itoa(i, r, 10);
    WritePrivateProfileString(appname.c_str(), kk, r, file_path.c_str());
  }

public:
  template <typename T>
  self& operator[](const T& aa){
    appname = lexical_cast<std::string>(aa); return *this;
  }

  template <typename A, typename B>
  self& set(const A& a, const B& b) {
    set_str(lexical_cast<std::string>(a).c_str(),
      lexical_cast<std::string>(b).c_str());
    return *this;
  }

  template <typename A, typename B>
  self& get(const A& a, B& b) {
    char buf[MAX] = { 0 };
    get_str(lexical_cast<std::string>(a).c_str(), buf);
    if (strlen(buf)) {
      b = lexical_cast<B>(buf);
    }
    return *this;
  }

  template <typename A, typename B>
  self& add(const A& a, B& b) {
    vib.push_back(inibind(lexical_cast<std::string>(a), b));
    return *this;
  }

  void save() {
    for (size_t i = 0; i < vib.size(); ++i) {
      vib[i].save(*this);
    }
  }
  void load() {
    for (size_t i = 0; i < vib.size(); ++i) {
      vib[i].load(*this);
    }
  }

  static void test() {
    cini ii   ("C:\\a.ini");
    double bb = 12;
    ii[999999];
    ii.set("asdf", "fdf");
    std::string a;
    ii.get("asdf", a);
    ii.add(23, bb);
    ii.add("asdf", bb);
    ii.save();
    //cout << bb;
  }
};


#endif // _CINI_HPP_
