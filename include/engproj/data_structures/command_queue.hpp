#pragma once

#include <cassert>
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

  void push(command_type& command){
    write_buffer_.emplace_back(std::move(command));
  }

  void push_commands(std::vector<command_type>& commands){//consumes vector
    std::scoped_lock lock(mtx_);
    write_buffer_.reserve(write_buffer_.size() + commands.size());
    std::move(commands.begin(),commands.end(), std::back_inserter(write_buffer_));
    commands.clear();
  }

  std::vector<command_type> extract_commands_and_swap(){
    //single consumer no lock neccessary, must be called by consumer after producers finish
    assert("no producers can be writing here" && !mtx_.try_lock());
    auto temp = std::move(read_buffer_);
    std::swap(write_buffer_,read_buffer_);
    write_buffer_.clear();
    return std::move(temp);
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



};

}
