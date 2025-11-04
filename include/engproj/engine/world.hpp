#pragma once

#include "engproj/engine/component/component.hpp"
#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/engine/component/view.hpp"
#include "engproj/engine/handle.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/logger/logger.hpp"
#include <utility>

namespace engproj::engine{


class world{
public:
  world(entity_hdl_mngr& entity_manager) : entity_manager_(entity_manager){}

  [[nodiscard]]
  std::optional<entity_hdl> create_entity(){
    auto temp = entity_manager_.create_handle();
    if(temp){
      return *temp;
    }else{
      return temp;
    }
  }

  //Delete copy and move operations
  world(const world&) = delete;            // copy constructor
  world& operator=(const world&) = delete; // copy assignment
  world(world&&) = delete;                 // move constructor
  world& operator=(world&&) = delete;      // move assignment

  bool destroy_entity(entity_hdl handle){
    if(entity_exists(handle)){
      deferred_destroy.push_back(handle);
      return true;
    }else{
      return false;
    }
  }

  bool entity_exists(entity_hdl handle){
    return entity_manager_.has_handle(handle);
  }

  template<typename T,typename... Args>
  T* add_component(entity_hdl handle, Args&&... args){
    static_assert(is_valid_component<T>(), "Tried to add invalid component type to world");
    if(!entity_exists(handle)){
      logger::e_logger.debug("Tried to add component to handle that handle manager says is invalid. Entity id:{},gen:{}",handle.id,handle.gen);
      return nullptr;
    }
    return get_pool<T>().add_component(handle, std::forward<Args>(args)...);
  }

  template<typename T>
  T* get_component(entity_hdl handle){
    static_assert(is_valid_component<T>(), "Tried to add invalid component type to world");
    if(!entity_exists(handle)){
      logger::e_logger.debug("Tried to get component of handle that handle manager says is invalid. Entity id:{},gen:{}",handle.id,handle.gen);
      return nullptr;
    }
    return get_pool<T>().get_component(handle);
  }

  template<typename... Components>
  component::view<Components...> get_view(){
    static_assert(sizeof...(Components) > 0, "View must have at least one component");
    static_assert(are_valid_components<Components ...>(), "Invalid component type in view");

    return component::view<Components ...>(get_pool<Components>()...);
  }

  template<typename... Components>
  component::view<Components...> get_view() const{
    static_assert(sizeof...(Components) > 0, "View must have at least one component");
    static_assert(are_valid_components<Components ...>(), "Invalid component type in view");

    return component::view<Components ...>(get_pool<Components>()...);
  }

  auto get_all_pools(){
    return std::tie(get_pool<component::transform>(),
                           get_pool<component::camera>());
  }

  template<std::size_t... Is>
  void end_frame(std::index_sequence<Is...>){
    auto pools = get_all_pools();
    while(!deferred_destroy.empty()){
      auto entity = deferred_destroy.back();
      (std::get<Is>(pools).remove_component(entity),...);
      entity_manager_.destroy_handle(entity);
      deferred_destroy.pop_back();
    }
  }

  void end_frame(){
    end_frame(std::make_index_sequence<pool_count_>{});
  }

private:
  entity_hdl_mngr& entity_manager_;
  std::vector<entity_hdl> deferred_destroy;
  component::componentpool<component::transform> transform_pool_;
  component::componentpool<component::camera> camera_pool_;
  //you must add all the pools
  static constexpr std::size_t pool_count_ = 2;

  template<typename T>
  component::componentpool<T>& get_pool();//constexpr or not? idk

  template<typename T>
  const component::componentpool<T>& get_pool() const;


  template<typename T>
  static constexpr bool is_valid_component();

  template<typename... Components>
  static constexpr bool are_valid_components();

};


template<typename T>
component::componentpool<T>& world::get_pool(){//constexpr or not? idk
  if constexpr(std::is_same_v<T,component::transform>){
    return transform_pool_;
  }
  else if constexpr(std::is_same_v<T,component::camera>){
    return camera_pool_;
  }
}

template<typename T>
const component::componentpool<T>& world::get_pool() const{//would this even work?
  return const_cast<world*>(this)->get_pool<T>();
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
  return (is_valid_component<Components>() && ...);
}

}
