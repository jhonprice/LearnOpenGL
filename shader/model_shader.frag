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

//�۲��
uniform vec3 eyePos;


void main()
{
    //��������
    vec3 ambient = vec3(texture(material.texture_diffuse1, TexCoords)) * light.ambient;

    
    //���������
    
    vec3 lightDir = normalize(light.position-FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    //�����Եƽ������
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //ƽ���߽����

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords));

    //���淴��
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(texture(material.texture_specular1, TexCoords)) * spec * light.specular; 

    //���˥��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    		    light.quadratic * (distance * distance));  
    
	vec3 result = ambient + diffuse*intensity + specular*intensity;
    //����Ӧ�õ���diffuse0.8 ambient 0.1 �������͵���£�����ǿ��ĸо�
    FragColor = vec4(result*attenuation, 1.0);
    

}