#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 Normal;
out vec3 FragPos;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //没有非均匀缩放可以直接使用原有法线
    Normal = mat3(transpose(inverse(model))) * aNormal;  //aNormal;
    //在世界空间进行光线计算
    FragPos = vec3(model * vec4(aPos, 1.0));
} 