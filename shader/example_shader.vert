#version 450 core
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    

    vec2 someVec = vec2(0.5, 1.0);
    vec4 differentVec = someVec.xyxx; //(0.5,1.0,0.5,0.5)
    vec3 anotherVec = differentVec.zyw; //(0.5,1.0,0.5)
    vec4 otherVec = someVec.xxxx + anotherVec.yxzy; //(0.5+1,0.5+0.5,0.5+0.5,0.5+1.0) (1.5,1,1,1.5)


    vertexColor = otherVec.xyzw-ivec4(1,1,1,1)+vec4(0,0,0,0.5);//vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}