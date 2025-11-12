//commands that the command queue will hold
#pragma once

#include "engproj/data_structures/command_queue.hpp"
#include "engproj/gl_utils/renderer/render_cmd.hpp"
#include "engproj/gl_utils/renderer/render_pass.hpp"
#include "engproj/gl_utils/renderer/fg_resource.hpp"
#include "engproj/logger/logger.hpp"
#include <string>
#include <unordered_map>

namespace engproj::gl_utils::renderer{

class framegraph{
public:
  framegraph(engproj::data_structures::command_queue<renderer::render_cmd>& command_queue,engproj::engine::world& world);
  framegraph(const framegraph&) = delete;
  framegraph& operator=(const framegraph&) = delete;
  framegraph(framegraph&&) = delete;
  framegraph& operator=(framegraph&&) = delete;

  template<typename Render_pass, typename... Args>
  Render_pass& add_pass(const std::string& name, Args&&... args){
    dirty_bit_=true;
    static_assert(std::is_base_of_v<renderer::render_pass, Render_pass>,"Render_pass must derive from renderer::render_pass");
    auto pass = std::make_unique<Render_pass>(std::forward<Args>(args)...);
    pass->name_ = name;
    Render_pass& ref = *pass;//this is fine, std::move moves the ptr not the object
    passes_.push_back(std::move(pass));
    return ref;
  }

  void compile(){
    if(!dirty_bit_){
      return;
    }
    for(auto& pass : passes_){
      pass->register_resources(*this);
    }



    dirty_bit_ = false;
  }
  void execute(){
  }

  renderer::fg_resource& get_resource(const std::string resource_name){
    auto it = resources_.find(resource_name);
    if(it!=resources_.end()){
      return it->second;
    }else{
      //log an error
      return dummy_;//don't return dummy_ fix this later
    }
  }

  template<typename... Args>
  renderer::fg_resource& create_resource(const std::string resource_name, Args&&... args){
    auto [it,inserted] = resources_.try_emplace(resource_name,std::forward<Args>(args)...);
    if(inserted){
      dirty_bit_ = true;
    }
    return it->second;//if exists return what we already have
  }



private:
  engproj::data_structures::command_queue<renderer::render_cmd>& command_queue_;
  const engproj::engine::world& world_;
  std::vector<std::unique_ptr<renderer::render_pass>> passes_;
  std::unordered_map<std::string, renderer::fg_resource> resources_;
  //std::vector<std::vecto> execution_order_;
  //present_pass_
  renderer::fg_resource dummy_;
  bool dirty_bit_ = true; //dirty if framegraph modified

};

}
