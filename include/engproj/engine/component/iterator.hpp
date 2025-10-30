#pragma once
#include "engproj/engine/component/componentpool.hpp"
#include <tuple>

namespace engproj::engine::component{

template<typename maincomponent, typename... components>
class iterator;


template<typename maincomponent>
class iterator<maincomponent>{//specialisation
public:
  iterator(componentpool<maincomponent>& component_pool) : component_pool_(component_pool){
    index_= component_pool_.get_entities[0];
  }


private:
  size_t index_;
  componentpool<maincomponent>& component_pool_;

};

template<typename maincomponent, typename... components>
class iterator{
public:


private:
  componentpool<maincomponent>& primary_component_pool_;
  std::tuple<componentpool<components>&...> pools_;


};

}
