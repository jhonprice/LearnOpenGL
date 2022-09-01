#version 450 core
out vec4 FragColor;

//法线信息
in vec3 Normal;
//照射点信息
in vec3 FragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
//光源
uniform vec3 lightPos;
//观察点
uniform vec3 eyePos;

void main()
{
    //环境光照
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    //总光照
    vec3 result = (ambient + diffuse+specular) * objectColor;
    FragColor = vec4(result, 1.0);
}