//commands that the command queue will hold
#pragma once

namespace engproj::gl_utils::renderer{

class render_cmd{
public:
  enum class type{
    bind_pipeline
  };


private:
  type type_;

};

}
