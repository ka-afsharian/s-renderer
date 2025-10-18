#pragma once
#include "buffertype.hpp"
#include <filesystem>
#include <string>
#include <vector>
#include <map>

namespace engproj::gl_utils{

struct uniformfield{
  std::string name;
  uint32_t type;
  int32_t offset;
  int32_t arraysize;
  int32_t arraystride;
  int32_t matrixstride;
  bool isrowmajor;
};


struct structnode{
  std::string name;
  std::vector<uniformfield> fields;
  std::map<std::string,structnode> nested;

};

struct bufferreflection{
  std::string name;
  int32_t binding;
  int32_t datasize;
  structnode root;
};


class shader_stage{
      public:

        enum class type {vertex,tesscontrol,tesseval,geometry,fragment,compute};//,task,mesh};

        explicit shader_stage(const type stage_type,std::filesystem::path path);
        explicit shader_stage(const type stage_type,std::string source);

        virtual ~shader_stage();

        shader_stage(const shader_stage&) = delete;
        shader_stage& operator=(const shader_stage&) = delete;
        shader_stage(shader_stage&&) = delete; //make one later noexcept
        shader_stage& operator=(shader_stage&&) = delete; //make one later noexcept




        uint32_t getid();

        int compile();//mv

        std::optional<std::vector<bufferreflection>> reflect(buffertype interfacetype = buffertype::ubo);

        std::string_view get_compile_log() const;//mv
        virtual std::string_view get_link_log() const;//mv

        void delete_program();//mv

        std::size_t get_source_size() const;

        bool is_valid() const{
             return is_valid_;
        }

        bool is_source_valid() const{
             return source_valid_;
        }

        std::optional<std::vector<bufferreflection>> get_ubo_reflections() const;
        std::optional<std::vector<bufferreflection>> get_ssbo_reflections() const;

      private:
        std::filesystem::path path_;
        int load_file(std::filesystem::path path);
        int load_string(std::string source);

      protected:
        std::string compile_log_;
        std::string link_log_;
        std::string source_;
        bool is_valid_ = false;
        bool source_valid_ = false;
        class gluint_PIMPL;
        std::unique_ptr<gluint_PIMPL> id_;
        const type stage_type_;
        std::optional<std::vector<bufferreflection>> ubo_reflection_;
        std::optional<std::vector<bufferreflection>> ssbo_reflection_;

};

}//end namespace
