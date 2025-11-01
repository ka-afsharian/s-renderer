#pragma once
#include "engproj/engine/component/iterator.hpp"
#include <any>
#include <variant>

namespace engproj::engine::component{

template<typename component>
class iterablerange{
public:
  iterablerange(componentpool<component> component_pool) : component_pool_(component_pool) {}

  iterator<component> begin(){
    return iterator<component>(0,component_pool_);
  }

  iterator<component> end(){
    return iterator<component>(component_pool_.size(),component_pool_);
  }
private:
  componentpool<component>& component_pool_;
};

template<typename... components>
class iterablerange_multi{
public:
  iterablerange_multi(std::tuple<componentpool<components>&...> pools) : pools_(pools),smallest_entities_(nullptr){
    auto pool_sizes = std::array{std::get<componentpool<components>&>(pools_).size()...};
    auto it = std::min_element(pool_sizes.begin(),pool_sizes.end());
    size_t smallest_pool_index = std::distance(pool_sizes.begin(),it);

    set_smallest_entities_at_index(smallest_pool_index, std::index_sequence_for<components...>{});
    size_ = (*smallest_entities_).size();
  }

  auto begin(){
    return iterator_multi<components...>(0,pools_,*smallest_entities_);
  }

  auto end(){
    return iterator_multi<components...>(size_,pools_,*smallest_entities_);
  }

private:
  std::tuple<componentpool<components>&...> pools_;
  std::vector<entity_hdl>* smallest_entities_;
  size_t size_;


  template<std::size_t... Is>
  void set_smallest_entities_at_index(std::size_t index, std::index_sequence<Is ...>){
    ((Is == index? smallest_entities_= &std::get<Is>(pools_).entities() : void() ),...);
  }

};

}
