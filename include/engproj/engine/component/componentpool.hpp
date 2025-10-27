#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/engine/handle_mngr.hpp"

#include <limits>
#include <optional>
#include <vector>
#include <cstdint>

namespace engproj::engine{

template<typename T>//T is a componenet
class componentpool{
  static inline const handle_id_type max_entities_ =handle_mngr<handletype::entity>::max_entries;
  static inline const handle_id_type invalid_entity_id_ = engproj::engine::entity_hdl::invalid.id;
public:
  componentpool() : entity_to_index_(1000,invalid_entity_id_){

  //must be preallocated, all values invalid ^
  }

  template<typename... Args>
  std::optional<T&> add_component(entity_hdl entity, Args&&... args){ //its perfectly fine to return reference, pools aren't thread safe, only world is
    if(!entity_valid(entity) || has_component(entity)){
      return std::nullopt;
    }
    ensure_capacity(entity); //grows the vector if neccessary

    size_t index = entities_.size();
    entities_.push_back(entity);
    components_.push_back(std::forward<Args>(args)...);
    entity_to_index_[entity.id] = index;

    return components_.back();
  }


private:
  std::vector<T> components_;
  std::vector<entity_hdl> entities_;//this and components_ are always the same size and index of each are the same for the same component
  std::vector<std::size_t> entity_to_index_; //size_t at this vectors index, is the index of components_
  //entity_to_index_ is sparse, so its going to take up alot of space 500kb per pool

  void ensure_capacity(const entity_hdl entity){    //
    if(entity.id >= entity_to_index_.size()){
      std::size_t new_size = std::max(entity.id + 1,max_entities_);
      entity_to_index_.resize(new_size,invalid_entity_id_);
    }
  }

  bool entity_valid(const entity_hdl entity) const{//checks if the entity struct itself is valid
    if(entity.id >= max_entities_ || entity.id == invalid_entity_id_){
      return false;
    }
    return true;
  }

  bool entity_within_sparse_size(const entity_hdl entity){//checks if entity id is within sparse size
    if(entity.id >= entity_to_index_.size()){
      return false;
    }
    return true;
  }

  bool has_component(const entity_hdl entity){
    if(!entity_valid(entity) || !entity_within_sparse_size(entity)){//checks if entity is valid and possible to be in sparse
      return false;
    }
    size_t index = entity_to_index_[entity.id];
    if(index == invalid_entity_id_){//checks if index isn't 0
      return false;
    }
    return entities_[index].gen == entity.gen; //if generation matches then entity already has component
  }

};


}
