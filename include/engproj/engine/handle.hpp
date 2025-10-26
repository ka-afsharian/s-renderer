#pragma once


#include <cstdint>
namespace engproj::engine{


enum class handletype{
   pipeline,
   buffer,
   texture,
   mesh,
   entity,
   material
};

//don't know whether to put this class in engine or glutils folder
template<handletype T>
struct handle{
  uint32_t id=0;
  uint32_t gen=0;//generation

  bool operator ==(const handle<T>& other) const{
    return id==other.id && gen==other.gen;
  }

  bool operator !=(const handle<T>& other) const{
    return !(*this == other);
  }

  struct hash{
    std::size_t operator()(const handle<T>& e) {
      return (static_cast<std::size_t>(e.id) << 32) | e.gen;
    }
  };

  static const handle<T> invalid;

};

template<handletype T>
const handle<T> handle<T>::invalid{0,0};

//creating types
using pipeline_hdl = engine::handle<handletype::pipeline>;
using buffer_hdl = engine::handle<handletype::buffer>;
using texture_hdl = engine::handle<handletype::texture>;
using mesh_hdl = engine::handle<handletype::mesh>;
using material_hdl = engine::handle<handletype::material>;
using entity_hdl = engine::handle<handletype::entity>;

}
