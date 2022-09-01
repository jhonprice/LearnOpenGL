#version 450 core
out vec4 FragColor;

//������Ϣ
in vec3 Normal;
//�������Ϣ
in vec3 FragPos; 
//<<��������>>
in vec2 TexCoords;

//<<�����ɫ>>
struct Light {
    //��Դ
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 

//�۲��
uniform vec3 eyePos;
//<<����>>
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
uniform Material material;

void main()
{
    //��������
    //float ambientStrength = 0.1;
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    //���������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    //float diffusePower = 1;
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));


    //���淴��
    //float specularStrength = 0.5;
    //float shininess = 32;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular; 

    //�ܹ���
    //vec3 result = (ambient + diffuse+specular) * objectColor;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}