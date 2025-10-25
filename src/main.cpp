
#include "glad/glad.h" //loads function pointers
#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>
//#include <SDL3/SDL_opengl.h>
//#include <GLFW/glfw3.h> //windowing library
//#include <GL/glut.h> //old alternative to glfw
//#include <GL/glew.h> //old alternative to glad
//#include <SDL3/SDL_video.h>
//#include <SDL3/SDL_video.h>
#include <iostream>
#include <stdio.h>
//#include <glm/glm.hpp>//math library
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <cmath>
//#include <string>
#include "engproj/gl_utils/shader_stage.hpp"
#include "engproj/gl_utils/shader.hpp"
#include "engproj/logger/logger.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engproj/gl_utils/manager.hpp"
#include <memory>
//#include <engproj/data_structures/boundedLRUmap.hpp>

int main(){
    //engproj::logger::logger<engproj::logger::loglevel::debug> logger{"main"};
    using namespace engproj::gl_utils;
    auto mainmngr = std::make_shared<manager>();
    mainmngr->init();
    mainmngr->print_hardware_props();
    auto context = mainmngr->get_context("main");

    context->activate_viewport("full");
    //glViewport(0,0,1920,1080);

    glClearColor(0.2f,0.3f,0.3f,1.0f);
//-----------------
//
    glm::vec3 Position = glm::vec3(-1.0f,2.2f,5.0f);
    glm::vec3 WorldUp = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 Front = glm::vec3(0.0f,-0.3f,-1.0f);
    glm::vec3 Right = glm::normalize(glm::cross(Front,WorldUp));
    glm::vec3 Up = glm::normalize(glm::cross(Right,Front));
    glm::mat4 viewmatrix = glm::lookAt(Position,Position+Front,Up);


    glm::vec3 lightPos(1.2f,1.0f,2.0f);

    glEnable(GL_DEPTH_TEST);

    auto mngr = std::make_shared<engproj::gl_utils::shader_stage_mngr>(100000,5000);
    mngr->warmup(std::vector<std::string>{"lighting_maps.vs","lighting_maps.fs","light_cube.vs","light_cube.fs"});
    engproj::gl_utils::pipeline_mngr pipemngr{50000,mngr};
    auto mappipeline = pipemngr.add("mappipeline",{{shader_stage::type::vertex,"lighting_maps.vs"},{shader_stage::type::fragment,"lighting_maps.fs"}});
    auto cubepipeline = pipemngr.add("cubepipeline",{{shader_stage::type::vertex,"light_cube.vs"},{shader_stage::type::fragment,"light_cube.fs"}});
    if(mappipeline){
      engproj::logger::g_logger.debug("VALID");
    }else{engproj::logger::g_logger.debug("INVALID");}

    auto vsid = (*mappipeline)->get_stage_id(shader_stage::type::vertex);
    auto fsid = (*mappipeline)->get_stage_id(shader_stage::type::fragment);
    auto vs2id = (*cubepipeline)->get_stage_id(shader_stage::type::vertex);
    auto fs2id = (*cubepipeline)->get_stage_id(shader_stage::type::fragment);

    mngr->cleanup();
    mngr->get_stats();
    mngr->debug_print();
    //engproj::logger::g_logger.debug("mappipelines size: {}",(*mappipeline)->get_source_size());
    (*mappipeline)->printubos();
    (*mappipeline)->printssbos();



    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int loadTexture(const char *path);

    (*cubepipeline)->use();
    (*mappipeline)->use();
    unsigned int diffuseMap = loadTexture(std::filesystem::path("../data/textures/container2.png").c_str());
    unsigned int specularMap = loadTexture(std::filesystem::path("../data/textures/container2_specular.png").c_str());





    int height=1080,width=1920;//for sdl window size change

    glProgramUniform1i(fsid,glGetUniformLocation(fsid,"material.diffuse"),0);
    glProgramUniform1i(fsid,glGetUniformLocation(fsid,"material.specular"),1);

    //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    SDL_GL_SetSwapInterval(0);
    bool running = true;
    while(running){

        SDL_Event e;
        while (SDL_PollEvent(&e)){
          if (e.type == SDL_EVENT_QUIT){
            running = false;
          }else if(e.type == SDL_EVENT_WINDOW_RESIZED){
            SDL_GetWindowSize((SDL_Window*)context->window_->get_window_ptr(),&width,&height);
            context->window_->set_height(height);
            context->window_->set_width(width);
            context->refresh_viewport();
          }


        }
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        (*mappipeline)->use();
        glProgramUniform3fv(fsid,glGetUniformLocation(fsid,"light.position"),1,&lightPos[0]);
        glProgramUniform3fv(fsid,glGetUniformLocation(fsid,"viewPos"),1,&Position[0]);


        glProgramUniform3f(fsid,glGetUniformLocation(fsid,"light.ambient"),0.2f,0.2f,0.2f);
        glProgramUniform3f(fsid,glGetUniformLocation(fsid,"light.diffuse"),0.5f,0.5f,0.5f);
        glProgramUniform3f(fsid,glGetUniformLocation(fsid,"light.specular"),1.0f,1.0f,1.0f);


        glProgramUniform1f(fsid,glGetUniformLocation(fsid,"material.shininess"),64.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = viewmatrix;
        glProgramUniformMatrix4fv(vsid,glGetUniformLocation(vsid,"projection"),1,GL_FALSE,&projection[0][0]);
        glProgramUniformMatrix4fv(vsid,glGetUniformLocation(vsid,"view"),1,GL_FALSE,&view[0][0]);
        glm::mat4 model = glm::mat4(1.0f);
        float time = static_cast<float>(SDL_GetTicks())/100.0f;
        model = glm::rotate(model,time,glm::vec3(0.0f,1.0f,1.0f));
        model = glm::rotate(model,time,glm::vec3(-1.0f,0.0f,1.0f));
        glProgramUniformMatrix4fv(vsid,glGetUniformLocation(vsid,"model"),1,GL_FALSE,&model[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        (*cubepipeline)->use();

        glProgramUniformMatrix4fv(vs2id,glGetUniformLocation(vs2id,"projection"),1,GL_FALSE,&projection[0][0]);
        glProgramUniformMatrix4fv(vs2id,glGetUniformLocation(vs2id,"view"),1,GL_FALSE,&view[0][0]);
        model = glm::mat4(1.0f);
        model = glm::translate(model,lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glProgramUniformMatrix4fv(vs2id,glGetUniformLocation(vs2id,"model"),1,GL_FALSE,&model[0][0]);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);















        //SDL_GL_SwapWindow(window);
        context->window_->swap();

    }




    return 0;


}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format=1;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
