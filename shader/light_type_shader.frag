#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;


//Material
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

//Light
struct Light{
    vec3 position;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;
};
uniform Light light;

//观察点
uniform vec3 eyePos;


void main()
{
    //环境光照
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

        //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    //镜面反射
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular; 

    
	vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}