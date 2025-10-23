#include "engproj/gl_utils/shader_stage.hpp"
#include <fstream>
#include <filesystem>
#include <glm/glm.hpp>
#include <iostream>
#include <glad/glad.h>
#include <map>



using namespace engproj::gl_utils;


class shader_stage::shader_PIMPL{//this is to prevent namespace pollution from c library in header
      public:
      GLuint value = 0; //woops no underscore, whats done is done
};


shader_stage::shader_stage(const type stage_type, std::filesystem::path path) : stage_type_(stage_type){//fix initializer list l8r
    id_ = std::make_unique<shader_PIMPL>();
    id_->value = 0;
    if(load_file(path)==-1){
        source_valid_ = false;
    }
    ubo_reflection_=std::nullopt;
    source_valid_ = true;
};

shader_stage::shader_stage(const type stage_type, std::string string) : stage_type_(stage_type){
    id_ = std::make_unique<shader_PIMPL>();
    id_->value = 0;
    if(load_string(string)==-1){
        source_valid_ = false;
    }
    ubo_reflection_=std::nullopt;
    source_valid_ = true;
};

int shader_stage::load_file(std::filesystem::path path){
    source_valid_ = false;

    std::ifstream file(path, std::ios::binary);
    if(!file){
        return -1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    source_  = buffer.str();

    source_valid_ = true;
    return 0;
}


int shader_stage::load_string(std::string source){
    source_ = source;
    source_valid_ = true;
    return 0;
}


shader_stage::~shader_stage(){
    delete_program();
}

uint32_t shader_stage::getid(){
    return id_->value;
}

int shader_stage::compile(){
    is_valid_=false;
    compile_log_="";
    link_log_="";

    if(source_valid_ != true){
        return -4; //source not valid
    }

    GLuint shader= 0;
switch(stage_type_){
    case type::vertex:
    shader = glCreateShader(GL_VERTEX_SHADER);
    break;
    case type::tesscontrol:
    shader = glCreateShader(GL_TESS_CONTROL_SHADER);
    break;
    case type::tesseval:
    shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    break;
    case type::geometry:
    shader = glCreateShader(GL_GEOMETRY_SHADER);
    break;
    case type::fragment:
    shader = glCreateShader(GL_FRAGMENT_SHADER);
    break;
    case type::compute:
    shader = glCreateShader(GL_COMPUTE_SHADER);
    break;
}
/*
    case type::task:
    shader = glCreateShader(GL_TASK_SHADER_NV);
    break;
    case type::mesh:
    shader = glCreateShader(GL_MESH_SHADER_NV);
    break;
}
*/
    const char* sourcePtr = source_.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        GLint logLength;
        glGetShaderiv(id_->value, GL_INFO_LOG_LENGTH, &logLength);
        thread_local std::vector<char> log(logLength);
        glGetShaderInfoLog(id_->value, logLength, nullptr, log.data());
        compile_log_ = std::string_view(log.data(), logLength-1);

        glDeleteShader(shader);
        return -1; //not compiled
    }

    if(id_->value == 0){
        id_->value = glCreateProgram();//program creation
    }else{//previous program id set
        delete_program();//del old program
        id_->value = glCreateProgram();//program creation
    }
    if(id_->value == 0){ //program creation fail
        glDeleteShader(shader);
        id_->value =0;//clear value just in case
        return -2; //compiled but program creation failed
    }

    glProgramParameteri(id_->value,GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(id_->value, shader);
    glLinkProgram(id_->value);

    GLint linked = GL_FALSE;
    glGetProgramiv(id_->value, GL_LINK_STATUS, &linked);
    if(!linked){
        GLint loglength = 0;
        glGetProgramiv(id_->value, GL_INFO_LOG_LENGTH, &loglength);
        std::string log(loglength, '\0');
        glGetProgramInfoLog(id_->value, loglength, nullptr, log.data());
        link_log_ = log;

        glDeleteShader(shader);
        delete_program();//del old program
        return -3;//program linking failed
    }

    glDeleteShader(shader);
    is_valid_ = true;


    ubo_reflection_ = reflect(buffertype::ubo);
    ssbo_reflection_ = reflect(buffertype::ssbo);

    return 0;
}


void shader_stage::delete_program(){
    compile_log_="";
    link_log_="";
    is_valid_ = 0;
    if(id_->value != 0){
      glDeleteProgram(id_->value);
    }
    id_->value = 0;
}

std::string_view shader_stage::get_compile_log() const{
    return compile_log_;
}

std::string_view shader_stage::get_link_log() const{
    return link_log_;
}


std::size_t shader_stage::get_source_size() const{
  if(source_valid_){
    return source_.length();
  }else{
    return 0;
  }
}


std::optional<std::vector<bufferreflection>> shader_stage::reflect(buffertype interfacetype){
  if(!is_valid_){
    return std::nullopt;
  }
  GLenum blockinterface = (interfacetype == buffertype::ubo)? GL_UNIFORM_BLOCK : GL_SHADER_STORAGE_BLOCK;
  GLenum variableinterface = (interfacetype == buffertype::ubo) ? GL_UNIFORM : GL_BUFFER_VARIABLE;

  std::vector<bufferreflection> result;

  GLint numBO=0;

  glGetProgramInterfaceiv(id_->value,blockinterface, GL_ACTIVE_RESOURCES, &numBO);
  //std::cout << "num blocks: " << numBO <<std::endl;
  const GLenum blockprops[] = {
    GL_NAME_LENGTH,
    GL_BUFFER_BINDING,
    GL_BUFFER_DATA_SIZE,
    GL_NUM_ACTIVE_VARIABLES
  };

  const GLenum variableprops[] ={
    GL_TYPE,
    GL_OFFSET,
    GL_ARRAY_SIZE,
    GL_ARRAY_STRIDE,
    GL_MATRIX_STRIDE,
    GL_IS_ROW_MAJOR,
    GL_NAME_LENGTH
  };

  for(int blockindex =0;blockindex < numBO; blockindex++){
    GLint values[4];
    glGetProgramResourceiv(id_->value,blockinterface,blockindex,4,blockprops,4,nullptr,values);
    GLint namelen = values[0];
    GLint binding = values[1];
    GLint datasize = values[2];
    GLint numactive = values[3];

    std::string blockname(namelen,'\0');
    glGetProgramResourceName(id_->value,blockinterface,blockindex,namelen,nullptr,blockname.data());
    if (!blockname.empty() && blockname.back() == '\0'){
      blockname.pop_back();//remove trailing null
    }

    bufferreflection buf;
    buf.name = blockname;
    buf.binding = binding;
    buf.datasize = datasize;

    //std::cout << "blockname: " << blockname << " binding: " << binding << " datasize: " << datasize << std::endl;

    std::vector<GLint> variableindices(numactive);
    glGetProgramResourceiv(id_->value,blockinterface,blockindex,1,(const GLenum[]){GL_ACTIVE_VARIABLES},numactive,
                             nullptr,variableindices.data());

    for(GLint varindex : variableindices){
      GLint varvalues[7];
      glGetProgramResourceiv(id_->value,variableinterface,varindex,7,variableprops,7,nullptr,varvalues);

      GLenum type = varvalues[0];
      GLint offset = varvalues[1];
      GLint arraysize = varvalues[2];
      GLint arraystride = varvalues[3];
      GLint matrixstride = varvalues[4];
      bool isrowmajor = varvalues[5];
      GLint namelength = varvalues[6];

      std::string varname(namelength,'\0');
      glGetProgramResourceName(id_->value,variableinterface,varindex,namelength,nullptr,varname.data());
      if (!varname.empty() && varname.back() == '\0'){
        varname.pop_back();
      }

      uniformfield field{
        .name = varname,
        .type = type,
        .offset = offset,
        .arraysize = arraysize,
        .arraystride = arraystride,
        .matrixstride = matrixstride,
        .isrowmajor = isrowmajor
      };
      /*
      std::cout << "varname: " << field.name << "\n" <<
      "type" << field.type << "\n" <<
      "offset" << field.offset << "\n" <<
      "arraysize" << field.arraysize << "\n" <<
      "arraystride" << field.arraystride << "\n" <<
      "matrixstride" << field.matrixstride << "\n" <<
      "isrowmajor" << field.isrowmajor << "\n" << std::endl;
      */
      auto parts = std::vector<std::string>();
      size_t pos = 0, next;
      while ((next = varname.find('.', pos)) != std::string::npos) {
        parts.push_back(varname.substr(pos, next - pos));
        pos = next + 1;
      }
      parts.push_back(varname.substr(pos));

      structnode* node = &buf.root;
      for (size_t i = 0; i + 1 < parts.size(); ++i) {
        std::string key = parts[i];
        if (key.find('[') != std::string::npos)
          key = key.substr(0, key.find('['));
        node = &node->nested[key];
        node->name = key;
      }
      node->fields.push_back(std::move(field));
    }

    //std::cout << "------------" << std::endl;
    result.push_back(std::move(buf));

  }
  if(result.empty()){
    return std::nullopt;
  }
  return result;//can this be optimized

}




std::optional<std::vector<bufferreflection>> shader_stage::get_ubo_reflections() const{
  return ubo_reflection_;
}

std::optional<std::vector<bufferreflection>> shader_stage::get_ssbo_reflections() const{
  return ssbo_reflection_;
}





//-------
