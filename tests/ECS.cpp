#include "engproj/logger/logger.hpp"
#include "engproj/engine/world.hpp"
#include "engproj/engine/component/component.hpp"
#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/engine/component/iterator.hpp"
#include "engproj/engine/component/iterablerange.hpp"
#include <iostream>
using namespace engproj::engine;
int main(){
  entity_hdl_mngr handle_manager{};
  engproj::engine::world myworld{handle_manager};
  auto camera = myworld.create_entity();
  auto player = myworld.create_entity();
  handle_manager.debug_print();
  myworld.add_component<component::transform>(*camera, glm::vec3(0.0f,0.0f,0.0f),glm::quat(1.0f,0.0f,0.0f,0.0f),
                                              glm::vec3(1.0f,1.0f,1.0f));
  myworld.add_component<component::camera>(*camera, glm::vec3(),glm::vec3(),45.0f,1.8f,1.0f,6.0f);
  myworld.add_component<component::transform>(*player, glm::vec3(5.0f,5.0f,5.0f),glm::quat(1.0f,0.0f,0.0f,0.0f),
                                              glm::vec3(1.0f,1.0f,1.0f));

  auto myview = myworld.get_view<component::camera,component::transform>();

  for(auto [a,b,c] : myview.each()){
    (void)a;(void)b;
    c.position = glm::vec3(22.0f,22.0f,22.0f);
  }
  myworld.destroy_entity(*camera);
  myworld.end_frame();
  auto myview2 = myworld.get_view<component::camera,component::transform>();
  for(auto [a,b,c] : myview2.each()){
    (void)a;(void)b;
    engproj::logger::t_logger.debug("inside myview each loop enity{},camera{},transform{}",a.id,b.aspect,c.position.x);
  }
  auto myview3 = myworld.get_view<component::transform>();
  for(auto [a,b] : myview3.each()){
    engproj::logger::t_logger.debug("inside myview each loop enity{},transform{}",a.id,b.position.x);
  }
  handle_manager.debug_print();


}
