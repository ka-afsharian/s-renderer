#pragma once
#include <memory>
#include <string>
#include <vector>
#include "engproj/data_structures/boundedLRUmap.hpp"
#include "engproj/gl_utils/shader_stage.hpp"
#include <map>
#include <functional>
#include <optional>

namespace engproj::gl_utils{
using stage_ptr = std::shared_ptr<engproj::gl_utils::shader_stage>;

struct stage_char_size_pred{
  inline size_t operator()(const stage_ptr stage){
    return stage->get_source_size();
  }
};

struct default_loader{
  inline stage_ptr operator()(const std::string string){
    engproj::gl_utils::shader_stage::type type_;
    if(string.ends_with("vs")){
      type_=engproj::gl_utils::shader_stage::type::vertex;
    }else{//expand on this later to support different shaders
      type_=engproj::gl_utils::shader_stage::type::fragment;
    }
    return std::make_shared<engproj::gl_utils::shader_stage>(type_,std::filesystem::path("../data/shaders/"+string));
  }
};

class shader_stage_mngr{
public:
  using stage_cache_type = engproj::data_structures::boundedLRUmap<std::string,stage_ptr>;
  using loader_type = std::function<stage_ptr(std::string)>;
  explicit shader_stage_mngr(std::size_t stage_capacity,std::size_t uncompiled_stage_capacity, loader_type loader=default_loader{});

  std::optional<stage_ptr> load(const std::string& stage); //loads from loader and truns stage_ptr//helper function
  std::optional<stage_ptr> get(const std::string& stage);//finds shader in any of the maps, if not finds in directory


  void preload(const std::string&);//adds to uncompiled cache
  void preload(const std::vector<std::string>&);
  void warmup(const std::string&);//adds to compiled cache
  void warmup(const std::vector<std::string>&);//adds to compiled cache


  void clear();//clear all maps
  void clear_compiled();
  void clear_uncompiled();

  void cleanup(); //remove all with just 1 reference count(from here)
  void cleanup_compiled();
  void cleanup_uncompiled();

  std::size_t get_count() const; //returns how many pointers this class holds
  std::size_t get_compiled_cache_count() const;
  std::size_t get_uncompiled_cache_count() const;
  std::size_t get_permanent_cache_count() const;
  void reset_stats();
  void get_stats();

  void resize_compiled_cache(std::size_t new_capacity);
  void resize_uncompiled_cache(std::size_t new_capacity);

  //void set_eviction_callback(std::function<void(const std::string&, stage_ptr)> callback); //for much later on
  //void set_compilation_strategy(std::function<stage_ptr(const std::string&)> compiler);
  //void set_retrieval_strategy(std::function<stage_ptr(const std::string&)> retriever);
  //bool validate_integrity() const;  // Check for cache consistency

  std::size_t get_size() const;
  std::size_t get_compiled_cache_size() const;
  std::size_t get_uncompiled_cache_size() const;
  std::size_t get_permanent_cache_size() const;

  void debug_print();

  bool contains(const std::string& stage) const;
  bool remove(const std::string& stage);

  //void shrink_to_fit();  // Attempt to reduce memory usage by cleanup the maps (prob won't do anything)
  void debug_print() const;

  struct map_stats {
    std::size_t hits;
    std::size_t misses;
};

private:
  size_t stage_capacity_;//maximum capacity in stages_
  size_t uncompiled_stage_capacity_;
  stage_cache_type stages_;
  stage_cache_type uncompiled_stages_;
  loader_type loader_;
  std::map<std::string,stage_ptr> permanent_stages_;
  map_stats stats_;

  bool add_compiled(std::string,stage_ptr);//adds a shader to compiled (helper function)
};

//---------------------------------------------------
class pipeline_mngr;//forward dec

class pipeline{
public:
  friend pipeline_mngr;
  using pipeline_type = std::map<engproj::gl_utils::shader_stage::type,stage_ptr>;

  explicit pipeline();
  ~pipeline();
  bool use();
  uint32_t get_stage_id(shader_stage::type) const;
  size_t get_source_size() const;
  size_t get_stage_size(shader_stage::type) const;
  bool use_stage(engproj::gl_utils::shader_stage::type,stage_ptr);
  std::optional<stage_ptr> get_stage_ptr(shader_stage::type);
  void printubos();
  void printssbos();
private:
  pipeline_type pipeline_;
  class gluint_PIMPL;
  std::unique_ptr<gluint_PIMPL> id_;
  std::vector<bufferreflection> uboreflections_;
  std::vector<bufferreflection> ssboreflections_;
  void printstruct(const structnode& node, int indent =0);
};



using pipeline_ptr = std::shared_ptr<engproj::gl_utils::pipeline>;

struct pipeline_char_size_pred{
  inline size_t operator()(const pipeline_ptr pipeline){
    return pipeline->get_source_size();
  }
};

class pipeline_mngr{
public:
  using pipeline_cache_type = engproj::data_structures::boundedLRUmap<std::string,pipeline_ptr>;
  using pipeline_map = std::map<engproj::gl_utils::shader_stage::type,std::string>;
  using pipeline_map_store = std::map<std::string,pipeline_map>;
  pipeline_mngr(size_t capacity,std::shared_ptr<shader_stage_mngr> shader_mngr);
  std::optional<pipeline_ptr> get(const std::string& pipeline);//returns a pipeline
  std::optional<pipeline_ptr> add(std::string,pipeline_map);
  void cleanup();//remove pipeline_ptr with only 1 reference
private:
  size_t capacity_;
  std::shared_ptr<shader_stage_mngr> shader_mngr_;
  pipeline_cache_type pipelines_;
  pipeline_map_store permanent_;
};





}//end namespace
