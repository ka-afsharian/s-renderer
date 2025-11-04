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
    return std::tie(component_pool_.get_entities()[index_],component_pool_.get_components()[index_]);
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
class iterator_multi{//this can't work with stl algos since only some indicies are valid
public:
  iterator_multi(size_t index,std::tuple<componentpool<components>&...>& pools,const std::vector<entity_hdl>& entities)
       : pools_(pools),entities_(entities),index_(index),size_(entities_.size()){
    if(index_<size_ && !all_pools_have_component(std::index_sequence_for<components...>{})){
      next_valid();
    }
  }

  auto operator*(){
    return std::tuple_cat(std::forward_as_tuple(entities_[index_]),
                          std::forward_as_tuple(std::get<componentpool<components>&>(pools_).get_component_no_check(entities_[index_])...));
  }

  iterator_multi& operator++(){
    next_valid();
    return *this;
  }

  bool operator ==(const iterator_multi& other) const {
    return index_ == other.index_;
  }
  bool operator !=(const iterator_multi& other) const {
    return !(*this == other);
  }

private:
  std::tuple<componentpool<components>&...> pools_;
  const std::vector<entity_hdl>& entities_;
  size_t index_;
  size_t size_;

  void next_valid(){
    index_++;
    while(index_<size_){
      if(all_pools_have_component(std::index_sequence_for<components...>{})){
        break;
      }
      index_++;
    }
  }

  template<std::size_t... Is>
  bool all_pools_have_component(std::index_sequence<Is...>) const{
    return(... && std::get<Is>(pools_).has_component(entities_[index_]));
  }

};

}
