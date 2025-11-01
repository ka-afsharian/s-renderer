#pragma once
#include "engproj/engine/component/componentpool.hpp"
#include <tuple>

namespace engproj::engine::component{


template<typename maincomponent>
class iterator{
public:
  iterator(size_t index,componentpool<maincomponent>& component_pool) : index_(index), size_(component_pool.size()), component_pool_(component_pool){
  }

  auto operator*(){
    return std::make_tuple(component_pool_.get_entities()[index_],component_pool_.get_components()[index_]);
  }

  iterator& operator++(){
    ++index_;
    return *this;
  }

  bool operator ==(const iterator& other) const {
    return index_ == other.index_;
  }
  bool operator !=(const iterator& other) const {
    return !(*this == other);
  }
private:
  size_t index_;
  size_t size_;
  componentpool<maincomponent>& component_pool_;
};
//----------------------------------------------------------
template<typename... components>
class iterator_multi{
public:
  iterator_multi(size_t index,std::tuple<componentpool<components>&...> pools,std::vector<entity_hdl>& entities)
       : index_(index),pools_(pools),entities_(entities){

  }


private:
  std::tuple<componentpool<components>&...> pools_;
  std::vector<entity_hdl> entities_;
  size_t index_;
  size_t size_;


};

}
