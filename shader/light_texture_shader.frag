#version 450 core
out vec4 FragColor;

//法线信息
in vec3 Normal;
//照射点信息
in vec3 FragPos; 
//<<纹理坐标>>
in vec2 TexCoords;

//<<光的颜色>>
struct Light {
    //光源
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 

//观察点
uniform vec3 eyePos;
//<<材质>>
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

void main()
{
    //环境光照
    //float ambientStrength = 0.1;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    //float diffusePower = 1;
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));


    //镜面反射
    //float specularStrength = 0.5;
    //float shininess = 32;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular; 

    //总光照
    //vec3 result = (ambient + diffuse+specular) * objectColor;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}