#pragma once
#include "engproj/engine/handle.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <format>



namespace engproj::engine::component{

struct transform{
  glm::vec3 position;
  glm::quat rotation;
  glm::vec3 scale;

  std::string debug_string(){
    glm::vec3 euler = glm::eulerAngles(rotation);
    return std::format("Position:({},{},{}), Rotation (rad):(pitch:{},yaw:{},roll:{}), Scale:({},{},{})",
                       position.x,position.y,position.z,euler.x,euler.y,euler.z,scale.x,scale.y,scale.z);
  }
};

struct translate{
  glm::vec3 position;
};

struct mesh{
  engproj::engine::mesh_hdl mesh;
  engproj::engine::material_hdl material;//mult entities share material so makes sense to have material as a handle
  bool visible;
  bool cast_shadows=true;//maybe change this into a component
};

struct camera{
  glm::vec3 front;
  glm::vec3 up;
  float fov;
  float aspect;
  float znear;
  float zfar;
  //glm::mat4 viewproj;//the previous stuff might not be neccessary if i keep this
  std::string debug_string(){
    return std::format("Front:({},{},{}), Up:({},{},{}), fov:{}, aspect:{}, znear:{}, zfar:{}",
                       front.x,front.y,front.z,up.x,up.y,up.z,fov,aspect,znear,zfar);
  }
};


struct directionallight{//later
  glm::vec3 color;
  float intensity;
  glm::vec3 direction;
  bool castsshadows = true;

};


struct spotlight{
  glm::vec3 color;
  float intensity;
  glm::vec3 direction;
  float innercone; //radians
  float outercone; //radians
  bool castsshadows = true;
};

struct pointlight{
  glm::vec3 color;
  float intensity;
  float range;
  bool castsshadows = false;
};


}
