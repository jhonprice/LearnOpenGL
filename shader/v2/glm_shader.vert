#version 450 core

layout(location=0)in vec4 apos;

uniform mat4 trans;

out vec4 vertexColor;

void main()
{
    gl_Position = trans*vec4(apos.x, apos.y, apos.z, 1.0);
    vertexColor = vec4(apos.x,apos.y,0.7f,1.f);
} 