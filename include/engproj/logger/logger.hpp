#pragma once
#include <memory>
#include <print>
#include <string_view>
#include <string>
#include <vector>
namespace engproj::logger{

enum class loglevel{
trace,
debug,
info,
warn,
error,
fatal
};

class logsink{
public:
  virtual void write(std::string_view level, std::string_view rest) = 0;
protected:
  virtual ~logsink() = default;
};

class filesink : public logsink {
public:
//impliment later
};

class termsink : public logsink {
public:
  void write(std::string_view str,std::string_view rest) override{
    std::print("[{}] {}\n",str,rest);
  }
};

template<loglevel activelevel = loglevel::info>
class logger{
public:
  logger(){
    sinks_.emplace_back(std::make_shared<termsink>());//unfortunate shared ptr, even if never shared
  }

  template<typename... Args>
  void debug(std::format_string<Args...> fmt,Args&&... args){
    if constexpr(activelevel <= loglevel::debug){
      log(loglevel::debug, fmt,std::forward<Args>(args)...);
    }else{
      //empty, hopefully compiller deletes calls to empty functions
    }
  }

  template<typename... Args>
  void info(std::format_string<Args...> fmt, Args&&... args){
    if constexpr(activelevel <= loglevel::info){
      log(loglevel::info, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void trace(std::format_string<Args...> fmt, Args&&... args){
    if constexpr(activelevel <= loglevel::info){
      log(loglevel::trace, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void fatal(std::format_string<Args...> fmt, Args&&... args){
    if constexpr(activelevel <= loglevel::info){
      log(loglevel::fatal, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void warn(std::format_string<Args...> fmt, Args&&... args){
    if constexpr(activelevel <= loglevel::warn){
      log(loglevel::warn, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void error(std::format_string<Args...> fmt, Args&&... args){
    if constexpr(activelevel <= loglevel::error){
      log(loglevel::error, fmt, std::forward<Args>(args)...);
    }
  }

  void push_sink(std::shared_ptr<logsink> sink){
    sinks_.push_back(sink);
  }



private:
  std::vector<std::shared_ptr<logsink>> sinks_;

  template<typename... Args>
  void log(loglevel level, std::format_string<Args...> fmt, Args&&... args){
    for(auto& e : sinks_){
      e->write(to_string(level),std::format(fmt,std::forward<Args>(args)...));
    }
  }


  inline std::string to_string(loglevel lvl){
    if (lvl==loglevel::debug){
      return "debug";
    } else if  (lvl == loglevel::info){
      return "info";
    } else if  (lvl == loglevel::trace){
      return "trace";
    } else if  (lvl == loglevel::error){
      return "trace";
    } else if  (lvl == loglevel::fatal){
      return "trace";
    } else if  (lvl == loglevel::warn){
      return "trace";
    } else {
      return "unknown";
    }
  }

};




}
