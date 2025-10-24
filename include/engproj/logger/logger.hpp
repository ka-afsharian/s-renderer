#pragma once
#include <memory>
#include <mutex>
#include <print>
#include <string_view>
#include <string>
#include <chrono>
#include <vector>
namespace engproj::logger{

enum class loglevel{
fatal,
trace,
debug,
info,
warn,
error
};





class logsink{
public:
  virtual void write(std::string_view tempstamp,std::string_view logger_name,
                     std::string_view level,std::string_view rest) const = 0;
  virtual void flush() const = 0;
protected:
  virtual ~logsink() = default;
};

class filesink : public logsink {
public:
//impliment later
};






class termsink : public logsink {
public:
  void write(std::string_view timestamp,std::string_view logger_name,
             std::string_view level,std::string_view rest) const override{
    std::scoped_lock lock(mtx_);
    std::print("{} [{}] [{}] {}\n",timestamp,logger_name,level,rest);
  }
  void flush() const override{
    std::fflush(stdout);
  }
private:
  mutable std::mutex mtx_;
};







template<loglevel activelevel = loglevel::info>
class logger{
public:

  explicit logger(const std::string& name) : name_(name){
    static auto term_out = std::make_shared<termsink>();
    push_sink(term_out);
  }

  template<typename... Args>
  void debug(std::format_string<Args...> fmt,Args&&... args) const{
    if constexpr(activelevel <= loglevel::debug){
      log(loglevel::debug, fmt,std::forward<Args>(args)...);
    }else{
      //empty, hopefully compiller deletes calls to empty functions
    }
  }

  template<typename... Args>
  void info(std::format_string<Args...> fmt, Args&&... args) const{
    if constexpr(activelevel <= loglevel::info){
      log(loglevel::info, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void trace(std::format_string<Args...> fmt, Args&&... args) const{
    if constexpr(activelevel <= loglevel::trace){
      log(loglevel::trace, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void fatal(std::format_string<Args...> fmt, Args&&... args) const{
    if constexpr(activelevel <= loglevel::fatal){
      log(loglevel::fatal, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void warn(std::format_string<Args...> fmt, Args&&... args) const{
    if constexpr(activelevel <= loglevel::warn){
      log(loglevel::warn, fmt, std::forward<Args>(args)...);
    }
  }

  template<typename... Args>
  void error(std::format_string<Args...> fmt, Args&&... args) const{
    if constexpr(activelevel <= loglevel::error){
      log(loglevel::error, fmt, std::forward<Args>(args)...);
    }
  }

  void push_sink(std::shared_ptr<logsink> sink){
    std::scoped_lock lock(sinks_mutex_);
    sinks_.push_back(sink);
  }

  const std::string& getname() const{
    return name_;
  }

private:
  std::string name_;
  std::vector<std::shared_ptr<logsink>> sinks_;
  mutable std::mutex sinks_mutex_;

  template<typename... Args>
  void log(loglevel level, std::format_string<Args...> fmt, Args&&... args) const{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::format("{:%Y-%m-%dT%H:%M:%S}Z", std::chrono::floor<std::chrono::milliseconds>(now));
    auto lvlstr = to_string(level);
    auto message = std::format(fmt,std::forward<Args>(args)...);

    std::scoped_lock lock(sinks_mutex_);
    for(auto& e : sinks_){
      e->write(timestamp,name_,lvlstr,message);
      if(level<=loglevel::debug){
        e->flush();
      }
    }
  }


  static std::string to_string(const loglevel lvl){
    switch(lvl) {
        case loglevel::trace: return "trace";
        case loglevel::debug: return "debug";
        case loglevel::info: return "info";
        case loglevel::warn: return "warn";
        case loglevel::error: return "error";
        case loglevel::fatal: return "fatal";
        default: return "unknown";
    }
  }

};






extern logger<loglevel::debug> g_logger;
}
