#include "engproj/gl_utils/shader.hpp"
#include <glad/glad.h>
#include <iostream>
using namespace engproj::gl_utils;




class pipeline::gluint_PIMPL{//this is to prevent namespace pollution from c library in header
      public:
      GLuint value = 0;
};

pipeline::pipeline(){
  id_ = std::make_unique<gluint_PIMPL>();
  glGenProgramPipelines(1,&id_->value);
}


bool pipeline::use(){
  glBindProgramPipeline(id_->value);
  return true;
}

pipeline::~pipeline(){
  glDeleteProgramPipelines(1,&id_->value);
}

uint32_t pipeline::get_stage_id(shader_stage::type stage_type){
auto ptr = pipeline_[stage_type];
if(ptr){
  return ptr->getid();
}else{return 0;}
}


bool pipeline::use_stage(engproj::gl_utils::shader_stage::type stage_type,stage_ptr ptr){
  if(ptr && ptr->is_valid()){
    pipeline_[stage_type] = ptr;
    switch(stage_type){
    case shader_stage::type::vertex:
      glUseProgramStages(id_->value, GL_VERTEX_SHADER_BIT, ptr->getid());
      break;
    case shader_stage::type::tesscontrol:
      glUseProgramStages(id_->value, GL_TESS_CONTROL_SHADER_BIT, ptr->getid());
      break;
    case shader_stage::type::tesseval:
      glUseProgramStages(id_->value, GL_TESS_EVALUATION_SHADER_BIT, ptr->getid());
      break;
    case shader_stage::type::geometry:
      glUseProgramStages(id_->value, GL_GEOMETRY_SHADER_BIT, ptr->getid());
      break;
    case shader_stage::type::fragment:
      glUseProgramStages(id_->value, GL_FRAGMENT_SHADER_BIT, ptr->getid());
      break;
    case shader_stage::type::compute:
      glUseProgramStages(id_->value, GL_COMPUTE_SHADER_BIT, ptr->getid());
      break;
    }
    if(auto reflections = ptr->get_ubo_reflections()){
      uboreflections_.insert(uboreflections_.end(),reflections->begin(),reflections->end());
    }
    if(auto reflections = ptr->get_ssbo_reflections()){
      ssboreflections_.insert(ssboreflections_.end(),reflections->begin(),reflections->end());
    }
  }else{return false;}
  return true;
}




size_t pipeline::get_source_size() const{
  size_t total = 0;
  for(auto it = pipeline_.begin();it!=pipeline_.end();it++){
    total += get_stage_size(it->first);
  }
  return total;
}
size_t pipeline::get_stage_size(shader_stage::type ty) const{
  auto it = pipeline_.find(ty);
  if(it!=pipeline_.end()){
    return it->second->get_source_size();
  }else{return 0;}
}

std::optional<stage_ptr> pipeline::get_stage_ptr(shader_stage::type ty){
  auto it = pipeline_.find(ty);
  if(it!=pipeline_.end()){
    return it->second;
  }else{return std::nullopt;}
}

void pipeline::printstruct(const structnode& node, int indent){
    std::string pad(indent, ' ');
    for (auto& field : node.fields) {
        std::cout << pad << "- " << field.name
                  << " (offset=" << field.offset
                  << ", type=" << field.type << " , arraysize= " << field.arraysize
                  << " , arraystride= " << field.arraystride << " , matrixstride= " << field.matrixstride <<")\n";
    }
    for (auto& kv : node.nested) {
        std::cout << pad << "Struct: " << kv.first << "\n";
        printstruct(kv.second, indent + 2);
    }
}
void pipeline::printubos(){
    for (auto& ubo : uboreflections_) {
        std::cout << "UBO: " << ubo.name
                  << " (binding=" << ubo.binding
                  << ", size=" << ubo.datasize << ")\n";
        printstruct(ubo.root, 2);
    }
}

void pipeline::printssbos(){
    for (auto& ssbo : ssboreflections_) {
        std::cout << "SSBO: " << ssbo.name
                  << " (binding=" << ssbo.binding
                  << ", size=" << ssbo.datasize << ")\n";
        printstruct(ssbo.root, 2);
    }
}

//----------------
pipeline_mngr::pipeline_mngr(size_t capacity, std::shared_ptr<shader_stage_mngr> shader_mngr) : capacity_(capacity),
                                        shader_mngr_(shader_mngr),pipelines_(capacity,pipeline_char_size_pred{}){

}

std::optional<pipeline_ptr> pipeline_mngr::get(const std::string& pipeline){
  auto temp = pipelines_.get(pipeline);
  if(temp){
    return temp;
  }else{
    auto it = permanent_.find(pipeline);
    if(it != permanent_.end()){
      add(it->first,it->second);
      return pipelines_.get(pipeline);
    }else{
      return std::nullopt;
    }
  }
}

std::optional<pipeline_ptr> pipeline_mngr::add(std::string name,pipeline_map value){
  auto temp = std::make_shared<pipeline>();
  for(auto it : value){
    auto stageptr = shader_mngr_->get(it.second);
    if(stageptr){
      bool result = temp->use_stage(it.first,*stageptr);
      if(!result){return std::nullopt;}
    }else{
      return std::nullopt;}
  }
  permanent_[name]=value;
  pipelines_.insert(name,temp);
  return get(name);
}
