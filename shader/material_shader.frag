#version 450 core
out vec4 FragColor;

//������Ϣ
in vec3 Normal;
//�������Ϣ
in vec3 FragPos; 

//uniform vec3 objectColor;
//uniform vec3 lightColor;
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

void main()
{
    //��������
    //float ambientStrength = 0.1;
    vec3 ambient = material.ambient * light.ambient;

    //���������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);

    //float diffusePower = 1;
    vec3 diffuse = diff * light.diffuse * material.diffuse;


    //���淴��
    //float specularStrength = 0.5;
    //float shininess = 32;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular; 

    //�ܹ���
    //vec3 result = (ambient + diffuse+specular) * objectColor;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}