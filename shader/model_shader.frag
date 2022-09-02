#version 450 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;



//Light
struct Light{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 specular;
    vec3 diffuse;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

//观察点
uniform vec3 eyePos;


void main()
{
    //环境光照
    vec3 ambient = vec3(texture(material.texture_diffuse1, TexCoords)) * light.ambient;

    
    //漫反射光照
    
    vec3 lightDir = normalize(light.position-FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    //计算边缘平滑过渡
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //平滑边界计算

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords));

    //镜面反射
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(texture(material.texture_specular1, TexCoords)) * spec * light.specular; 

    //光的衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));  
    
	vec3 result = ambient + diffuse*intensity + specular*intensity;
    //本来应该调整diffuse0.8 ambient 0.1 但这里简单偷懒下，并加强光的感觉
    FragColor = vec4(result*attenuation, 1.0);
    

}