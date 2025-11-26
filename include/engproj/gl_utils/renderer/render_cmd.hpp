//commands that the command queue will hold
#pragma once

namespace engproj::gl_utils::renderer{

struct render_cmd{
  enum class type{
    bind_pipeline
  };
  type type_;

};

}
