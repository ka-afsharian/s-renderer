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

using handle_id_type = uint32_t;
using handle_gen_type = uint32_t;

template<handletype T>
struct handle{
  handle_id_type id=0;
  handle_gen_type gen=0;//generation

  handle(handle_id_type id_p,handle_gen_type gen_p) :id(id_p),gen(gen_p){
  }

  handle(const handle&) = default;
  handle(handle&&) noexcept = default;
  handle& operator=(const handle&) = default;
  handle& operator=(handle&&) noexcept = default;

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

  static inline const handle<T> invalid{0,0};

};


//creating types
using pipeline_hdl = engine::handle<handletype::pipeline>;
using buffer_hdl = engine::handle<handletype::buffer>;
using texture_hdl = engine::handle<handletype::texture>;
using mesh_hdl = engine::handle<handletype::mesh>;
using material_hdl = engine::handle<handletype::material>;
using entity_hdl = engine::handle<handletype::entity>;

}
