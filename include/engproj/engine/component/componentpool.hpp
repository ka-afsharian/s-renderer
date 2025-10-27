#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/logger/logger.hpp"

#include <optional>
#include <vector>
#include <cstdint>

namespace engproj::engine{

template<typename T>//T is a componenet
class componentpool{
  static inline const handle_id_type max_entity_id_ =handle_mngr<handletype::entity>::max_id_;//this is maybe 65536
  static inline const handle_id_type invalid_entity_id_ = handle_mngr<handletype::entity>::invalid_id_;//this is 0
  static const size_t invalid_index_ = max_entity_id_ + 1; //this should be fine
public:
  componentpool() : entity_to_index_(1000,invalid_index_){

  //must be preallocated, all values invalid ^
  //start with a 1000 or so?
  }

  template<typename... Args>
  std::optional<T&> add_component(entity_hdl entity, Args&&... args){ //its perfectly fine to return reference, pools aren't thread safe, only world is
    if(!entity_valid(entity) || has_component(entity)){
      logger::e_logger.debug("Tried to add component that already existed, or to invalid entity. Entity id:{}, gen:{}",entity.id,entity.gen);
      return std::nullopt;
    }
    ensure_capacity(entity); //grows the vector if neccessary

    size_t index = entities_.size();
    entities_.push_back(entity);
    components_.push_back(std::forward<Args>(args)...);
    entity_to_index_[entity.id] = index;//static cast entity.id to size_t ?

    logger::e_logger.debug("Added component to entity. Entity id:{}, gen:{}",entity.id,entity.gen);
    return components_.back();
  }


private:
  std::vector<T> components_;
  std::vector<entity_hdl> entities_;//this and components_ are always the same size and index of each are the same for the same component
  std::vector<handle_id_type> entity_to_index_; // at this vectors index, is the index of components_
  //entity_to_index_ is sparse, so its going to take up alot of space 500kb per pool
  //its confusing that the type is <handle_id_type>, just making sure to use as little space as possible

  void ensure_capacity(const entity_hdl entity){    //
    if(entity.id >= entity_to_index_.size()){
      size_t new_size = std::min(entity.id + 1,max_entity_id_+1);//kinda inefficient fix later
      entity_to_index_.resize(new_size,invalid_index_);
      logger::e_logger.debug("Resizing component pool to size {}. Entity id:{}, gen:{}",new_size,entity.id,entity.gen);
    }
  }

  bool entity_valid(const entity_hdl entity) const{//checks if the entity struct itself is valid
    if(entity.id > max_entity_id_ || entity.id == invalid_entity_id_){
      logger::e_logger.debug("Entity valid check false, componentpool class. Entity id:{}, gen:{}",entity.id,entity.gen);
      return false;
    }
    return true;
  }

  bool entity_within_sparse_size(const entity_hdl entity){//checks if entity id is within sparse size
    if(entity.id >= entity_to_index_.size()){
      logger::e_logger.debug("Entity within sparse size check false, componentpool class. Entity id:{}, gen:{}",entity.id,entity.gen);
      return false;
    }
    return true;
  }

  bool has_component(const entity_hdl entity){
    if(!entity_valid(entity) || !entity_within_sparse_size(entity)){//checks if entity is valid and possible to be in sparse
      return false;
    }
    size_t index = entity_to_index_[entity.id];
    if(index == invalid_index_){//checks if index isn't 0
      return false;
    }
    return entities_[index] == entity; //if generation matches then entity already has component. also does another id check
  }

};


}
