#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

/*
layout(std140, binding = 1) uniform CameraData {
    mat4 view;
    mat4 projection;
};
*/
layout(std430, binding = 0) buffer ParticleFixed {
    vec4 position[10];    // Fixed size array of 10 elements
    vec4 velocity[10];    // Fixed size array of 10 elements
} partic ;
layout(std430, binding = 1) buffer ParticleRuntime {
    int numParticles;     // scalar before runtime array
    mat4 transform;       // fixed-size struct
    vec4 position[];      // Runtime-sized array
} particrun;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
