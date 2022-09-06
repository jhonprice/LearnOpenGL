#version 450 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform int flag;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void reflectFunc(){
	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}

void refractFunc()
{             
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}  

void main()
{
    if(flag==1){
        reflectFunc();
    }else{
        refractFunc();
    }
}