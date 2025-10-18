#include "engproj/gl_utils/shader.hpp"
#include <iostream>


using namespace engproj::gl_utils;


shader_stage_mngr::shader_stage_mngr(std::size_t stage_capacity,std::size_t uncompiled_stage_capacity,loader_type loader) :
  stages_(stage_capacity,stage_char_size_pred{}), uncompiled_stages_(uncompiled_stage_capacity,stage_char_size_pred{}),
  loader_(loader){

  uncompiled_stage_capacity_ = uncompiled_stage_capacity;
  stage_capacity_ = stage_capacity;
  stats_={0,0};
}


std::optional<stage_ptr> shader_stage_mngr::load(const std::string& stage){
  stage_ptr loaded = loader_(stage);       //load source
  if(loaded->is_source_valid()){       //if source code good
    loaded->compile();                //then compile
    if(loaded->is_valid()){            //if valid
      auto vec_keyptrs = stages_.insert(stage,loaded);  //insert in compiled stages
      if(vec_keyptrs){                                 //if stuff was kicked out of compiled stages
        for(auto e : *vec_keyptrs){
          e.second->delete_program();                        //uncompile
          uncompiled_stages_.insert(e.first,e.second);         //insert in uncompiled stages(stuff might be kicked out from here too)
        }
      }else{return std::nullopt;} //stage_ rejected shader(prob cuz too big)
      return loaded;                                  //then return
    }else {return std::nullopt;}                   //if compilation broke return nullopt
  }else{
    return std::nullopt;
  }                                              //if source is invalid return nullopt
}

std::optional<stage_ptr> shader_stage_mngr::get(const std::string& stage){
  if(stages_.contains(stage)){//is it in main stages?
    stats_.hits++;
    return stages_.get(stage);
  }else if(uncompiled_stages_.contains(stage)){//is it in uncompoiled stages?
    stats_.hits++;
    auto temp = uncompiled_stages_.get(stage);
    uncompiled_stages_.erase(stage);
    int compiled = (*temp)->compile();
    if(compiled==0){//makes sure it doesn't get added back to uncompiled stages
      if(add_compiled(stage,*temp)==true && stages_.contains(stage)){
        return stages_.get(stage);
      }else{return std::nullopt;}
    }else{return std::nullopt;}
  }else if(permanent_stages_.contains(stage)){//is it in permanent stages?
    stats_.hits++;
    return(permanent_stages_[stage]);
  }else{  //if not
    stats_.misses++;
    return load(stage);
  }
}

bool shader_stage_mngr::add_compiled(std::string string,stage_ptr ptr){
  if(ptr && ptr->is_source_valid() && ptr->is_valid()){
    auto vec_keyptrs = stages_.insert(string,ptr);
    if(vec_keyptrs){
      for(auto e : *vec_keyptrs){
        e.second->delete_program();
        uncompiled_stages_.insert(e.first,e.second);
        }
    }else{return false;}
  }else{return false;}
  return true;
}

void shader_stage_mngr::preload(const std::string& stage){
  if(stages_.contains(stage) || uncompiled_stages_.contains(stage) || permanent_stages_.contains(stage)){//is it in stages?
  }else{  //if not
  stage_ptr loaded = loader_(stage);       //load source
  if(loaded->is_source_valid()){       //if source code good
      uncompiled_stages_.insert(stage,loaded);  //insert in uncompiled stages
  }
  }
}//adds to uncompiled cache
void shader_stage_mngr::warmup(const std::string& stage){
  get(stage);
}//adds to compiled cache



void shader_stage_mngr::preload(const std::vector<std::string>& stages){
  for(auto e : stages){
    preload(e);
  }

}
void shader_stage_mngr::warmup(const std::vector<std::string>& stages){
  for(auto e : stages){
    warmup(e);
  }
}

void shader_stage_mngr::clear_compiled(){
  stages_.clear();
}
void shader_stage_mngr::clear_uncompiled(){
  uncompiled_stages_.clear();
}
void shader_stage_mngr::clear(){
  clear_compiled();
  clear_uncompiled();
}

void shader_stage_mngr::cleanup_compiled(){
  std::vector<std::string> keys_to_erase;
  for(auto ptr = stages_.begin();ptr != stages_.end();ptr++){
    if(ptr->use_count()==1){
      keys_to_erase.push_back(ptr.key());
    }
  }
  for(const auto& key : keys_to_erase){
    stages_.erase(key);
  }
}

void shader_stage_mngr::cleanup_uncompiled(){
  std::vector<std::string> keys_to_erase;
  for(auto ptr = uncompiled_stages_.begin();ptr != stages_.end();ptr++){
    if(ptr->use_count()==1){
      keys_to_erase.push_back(ptr.key());
    }
  }
  for(const auto& key : keys_to_erase){
    uncompiled_stages_.erase(key);
  }
}

void shader_stage_mngr::cleanup(){
  cleanup_compiled();
  cleanup_uncompiled();
} //remove all with just 1 reference count(from here)


std::size_t shader_stage_mngr::get_compiled_cache_count() const{
  return stages_.get_size();
}
std::size_t shader_stage_mngr::get_uncompiled_cache_count() const{
  return uncompiled_stages_.get_size();
}
std::size_t shader_stage_mngr::get_permanent_cache_count() const{
  return permanent_stages_.size();
}
std::size_t shader_stage_mngr::get_count() const{
  return get_compiled_cache_count()+get_uncompiled_cache_count() + get_permanent_cache_count();
} //returns how many pointers this class holds
void shader_stage_mngr::reset_stats(){
  stats_ = {0,0};
}

void shader_stage_mngr::get_stats(){
  std::cout << "hits: " << stats_.hits << "\n";
  std::cout << "misses: " << stats_.misses << std::endl;
}

void shader_stage_mngr::resize_compiled_cache(std::size_t new_capacity){
  stages_.set_max_capacity(new_capacity);
}
void shader_stage_mngr::resize_uncompiled_cache(std::size_t new_capacity){
  uncompiled_stages_.set_max_capacity(new_capacity);
}

bool shader_stage_mngr::contains(const std::string& stage) const{
  return stages_.contains(stage);
}
bool shader_stage_mngr::remove(const std::string& stage){
  return stages_.erase(stage);
}

std::size_t shader_stage_mngr::get_compiled_cache_size() const{
  return stages_.get_capacity();
}

std::size_t shader_stage_mngr::get_uncompiled_cache_size() const{
  return uncompiled_stages_.get_capacity();
}
std::size_t shader_stage_mngr::get_permanent_cache_size() const{
  size_t total_size=0;
  for(const auto& e : permanent_stages_){
    total_size += e.second->get_source_size();
  }
  return total_size;
}
std::size_t shader_stage_mngr::get_size() const{
  return get_compiled_cache_size()+get_uncompiled_cache_size()+get_permanent_cache_size();
}


void shader_stage_mngr::debug_print(){
  std::cout << "Total cache count:" << get_count() << std::endl;
  std::cout << "Total cache use: " << get_size() << std::endl;
  std::cout << "---------------" <<std::endl;
  std::cout << "Compiled cache:" << std::endl;
  std::cout << "Compiled cache count: " << get_compiled_cache_count() << std::endl;
  std::cout << "Compiled cache size: " << get_compiled_cache_size() << std::endl;
  std::cout << "Contents:" << std::endl;
  for(auto ptr = stages_.begin();ptr != stages_.end();ptr++){
    std::cout << "name: " << ptr.key() << "\n";
    std::cout << "size " << (*ptr)->get_source_size() << std::endl;
  }
  std::cout << "---------------" <<std::endl;

  std::cout << "Uncompiled cache:" << std::endl;
  std::cout << "Uncompiled cache count: " << get_uncompiled_cache_count() << std::endl;
  std::cout << "Uncompiled cache size: " << get_uncompiled_cache_size() << std::endl;
  std::cout << "Contents:" << std::endl;
  for(auto ptr = uncompiled_stages_.begin();ptr != uncompiled_stages_.end();ptr++){
    std::cout << "name: " << ptr.key() << "\n";
    std::cout << "size " << (*ptr)->get_source_size() << std::endl;
  }
  std::cout << "---------------" <<std::endl;

  std::cout << "Permanent cache:" << std::endl;
  std::cout << "Permanent cache count: " << permanent_stages_.size() << std::endl;
  std::cout << "Permanent cache size: " << get_permanent_cache_size() << std::endl;
  std::cout << "Contents:" << std::endl;
  for(auto& e : permanent_stages_){
    std::cout << "name: " << e.first << "\n";
    std::cout << "size " << e.second->get_source_size() << std::endl;
  }
}




/*
private:
  loader_type loader_;
  cache_map stages_;
  cache_map uncompiled_stages_;
  std::map<std::string,stage_ptr> permanent_stages_;
  size_t stage_capacity_;
  size_t uncompiled_stage_capacity_;
  map_stats stats_;

};
*/
