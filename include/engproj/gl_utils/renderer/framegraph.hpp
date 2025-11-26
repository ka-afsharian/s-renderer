//commands that the command queue will hold
#pragma once

#include "engproj/data_structures/command_queue.hpp"
#include "engproj/gl_utils/renderer/render_cmd.hpp"
#include "engproj/gl_utils/renderer/render_pass.hpp"
#include "engproj/gl_utils/renderer/render_device.hpp"
#include "engproj/gl_utils/renderer/fg_resource.hpp"
#include "engproj/logger/logger.hpp"
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace engproj::gl_utils::renderer{

class framegraph{
public:
  using resource_ptr = std::shared_ptr<renderer::fg_resource>;
  using pass_ptr = std::unique_ptr<renderer::render_pass>;
  framegraph(engproj::data_structures::command_queue<renderer::render_cmd>& command_queue, const engproj::engine::world& world);
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
    auto [it,inserted] = passes_.insert({name,std::move(pass)});//add code to do something if same name exists in map
    if(inserted){
      return ref;
    }else{
      throw std::runtime_error("Render Pass name already exists in framegraph");
    }
  }

  void compile(){
    if(!dirty_bit_){
      return;
    }

    execution_list_.clear();//these three must be cleared
    marked_passes_.clear();
    resource_writers_.clear();
    cleanup_resources();

    for(auto& [name,pass] : passes_){
      pass->register_resources(*this);
      for(const auto& write_res : pass->writes_){
        if(resource_writers_.contains(write_res)){
          throw std::runtime_error("resource_writers_ already contains resource. framegraph class, compile function");
        }
        resource_writers_[write_res] = pass.get();//assumes each resource only has one pass writing to it, which should be the case
      }
    }

    std::vector<renderer::render_pass*> root_passes_;//passes which have 'present' usage, they are the final passes
    for (auto& [res,pass] : resource_writers_){
      if(resources_[res]->has_flag(renderer::fg_resource::usage_flag::present)){
        root_passes_.push_back(pass);
      }
    }

    //lambda that recursively marks passes needed and does topological sort for creation of execution list
    auto mark_pass_recursive = [&](auto&& self, renderer::render_pass* pass) -> void {
      if(!pass || marked_passes_.count(pass) > 0){
        return;
      }

      marked_passes_.insert(pass);

      for(const auto& read_res : pass->reads_){
        auto it = resource_writers_.find(read_res);
        if(it != resource_writers_.end()){
          self(self,it->second);
        }
      }

      execution_list_.push_back(pass);//this is good, execution_list will be in right order

    };

    for (auto* res : root_passes_){
      mark_pass_recursive(mark_pass_recursive,res);
    }

    //<---here use resoucemanager to initialize all the handles in marked passes
    dirty_bit_ = false;
  }

  void execute(){
    for(auto* pass : execution_list_){
      pass->execute(command_queue_,world_);
    }
  }

  resource_ptr get_resource(const std::string resource_name){
    auto it = resources_.find(resource_name);
    if(it!=resources_.end()){
      return it->second;
    }else{
      //log an error
      return nullptr;
    }
  }

  template<typename... Args>
  resource_ptr create_resource(const std::string resource_name, Args&&... args){
    auto [it,inserted] = resources_.try_emplace(resource_name,std::make_shared<renderer::fg_resource>(std::forward<Args>(args)...));
    if(inserted){
      dirty_bit_ = true;
      return it->second;
    }
    return it->second;
  }



private:
  engproj::data_structures::command_queue<renderer::render_cmd>& command_queue_;
  const engproj::engine::world& world_;
  std::unordered_map<std::string,pass_ptr> passes_;//unqiue ptr, framegraph owns the passes
  std::unordered_map<std::string, resource_ptr> resources_;//make a function that cleans up all resources with ref count 1
  std::unordered_map<std::string,renderer::render_pass*> resource_writers_;//for a resource, which render pass writes to it
  std::unordered_set<renderer::render_pass*> marked_passes_;
  std::vector<renderer::render_pass*> execution_list_;

  bool dirty_bit_ = true; //dirty if framegraph modified

  void cleanup_resources(){
    for (auto& [name,res] : resources_){
      if(res.use_count()==1){//means not also shared by a render_pass
        //use resourcemanager to delete the handle
        resources_.erase(name);
        resource_writers_.erase(name);
      }
    }
  }

};

}
