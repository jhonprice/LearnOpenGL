#version 450 core

layout(location=0)in vec4 apos;

uniform float aScale;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aScale * apos.x, aScale * apos.y, apos.z, 1.0);
    vertexColor = vec4(apos.x,apos.y,0.7f,1.f);
} 