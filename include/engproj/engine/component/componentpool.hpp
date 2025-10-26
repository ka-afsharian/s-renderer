#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/engine/handle_mngr.hpp"

#include <limits>
#include <vector>
#include <cstdint>

namespace engproj::engine{

template<typename T>//T is a componenet
class componentpool{
  static const std::size_t max_entities_ =handle_mngr<handletype::entity>::max_entries;
  static const std::size_t invalid_entity_ = std::numeric_limits<size_t>::max();
public:
  componentpool() : entity_to_index_(max_entities_,invalid_entity_){
  //must be preallocated, all values invalid ^

  }


private:
  std::vector<T> components_;
  std::vector<entity_hdl> entities_;//this and components_ are always the same size and index of each are the same for the same component
  std::vector<std::size_t> entity_to_index_; //size_t at this vectors index, is the index of components_
  //entity_to_index_ is sparse, so its going to take up alot of space 500kb per pool



};


}
