#pragma once

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <vector>
namespace engproj::data_structures{

//possibly multiple producers, one consumer
template <typename T>
class command_queue{
public:
  using command_type = T;

  command_queue() = default;
  command_queue(const command_queue&) = delete;
  command_queue& operator=(const command_queue&) = delete;
  command_queue(command_queue&&) = delete;
  command_queue& operator=(command_queue&&) = delete;

  template<typename U>
  void push(U&& command){
    std::scoped_lock lock(mtx_);
    write_buffer_.emplace_back(std::forward<U>(command));
  }

  template<typename Vec>
  void push_commands(Vec&& commands){//consumes vector
    std::scoped_lock lock(mtx_);
    write_buffer_.reserve(write_buffer_.size() + commands.size());
    if constexpr (std::is_rvalue_reference_v<decltype(commands)>){
      std::move(std::begin(commands),std::end(commands), std::back_inserter(write_buffer_));
    }else{
      std::copy(std::begin(commands),std::end(commands), std::back_inserter(write_buffer_));
    }
  }

  void register_thread(){
    std::scoped_lock lock(mtx_);
    thread_count_++;
    cv_.notify_one();
  }

  void deregister_thread(){
    std::scoped_lock lock(mtx_);
    thread_count_--;
    if(thread_count_ < 0){
      thread_count_ = 0;
    }
    cv_.notify_one();
  }

  void signal_thread(){
    std::scoped_lock lock(mtx_);
    signaled_count_++;
    cv_.notify_one();
  }

  std::vector<command_type> extract_commands_and_swap(){//there might be a problem with buffer capacities getting to big
    std::scoped_lock lock(mtx_);
    cv_.wait(lock,[&]{return thread_count_<=signaled_count_;});//only one consumer should ever be waiting here
    signaled_count_=0;
    auto temp = std::move(read_buffer_);
    std::swap(write_buffer_,read_buffer_);
    clear();
    return temp;
  }

  void clear(){
    std::scoped_lock lock(mtx_);
    write_buffer_.clear();
    read_buffer_.clear();
  }


private:
  std::vector<command_type> write_buffer_;
  std::vector<command_type> read_buffer_;
  std::mutex mtx_;
  std::condition_variable cv_;
  std::size_t thread_count_ = 0;
  std::size_t signaled_count_ = 0;

};

}
