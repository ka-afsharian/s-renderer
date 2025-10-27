#pragma once

#include "engproj/engine/component/component.hpp"
#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/handle.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/gl_utils/shader.hpp"

#include <cstdint>
#include <vector>
#include <map>


namespace engproj::engine{


class world{
  world(entity_hdl_mngr entity_manager) : entity_manager_(entity_manager){}

  entity_hdl create_entity();//use handle_mngr
  void destroy_entity(entity_hdl);//use handle_mngr
  bool is_valid(entity_hdl);//use handle_mngr

  template<typename T,typename... Args>
  T& add_component(entity_hdl){

  }


private:
  entity_hdl_mngr entity_manager_;
  componentpool<component::transform> transform_pool_;
  componentpool<component::camera> camera_pool_;


  template<typename T>
  componentpool<T>& get_pool();//constexpr or not? idk

  template<typename T>
  const componentpool<T>& get_pool() const;


  template<typename T>
  static constexpr bool is_valid_component();

  template<typename... Components>
  static constexpr bool are_valid_components();

};


template<typename T>
componentpool<T>& world::get_pool(){//constexpr or not? idk
  if constexpr(std::is_same_v<T,component::transform>){
    return transform_pool_;
  }
  else if constexpr(std::is_same_v<T,component::camera>){
    return camera_pool_;
  }
}

template<typename T>
const componentpool<T>& world::get_pool() const{
  return const_cast<world*>(this)->get_pool<T>;
}


template<typename T>
constexpr bool world::is_valid_component(){
  return std::is_same_v<T, component::transform> ||
         std::is_same_v<T, component::camera> ||
         std::is_same_v<T, component::directionallight> ||
         std::is_same_v<T, component::mesh> ||
         std::is_same_v<T, component::pointlight> ||
         std::is_same_v<T, component::spotlight> ||
         std::is_same_v<T, component::translate>;//add more if you create more components
}

template<typename... Components>
constexpr bool world::are_valid_components(){
  return (is_valid_components<Components>() && ...);
}

}
