#version 450 core
out vec4 FragColor;

//������Ϣ
in vec3 Normal;
//�������Ϣ
in vec3 FragPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
//��Դ
uniform vec3 lightPos;
//�۲��
uniform vec3 eyePos;

void main()
{
    //��������
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //���������
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    //���淴��
    float specularStrength = 0.5;
    vec3 viewDir = normalize(eyePos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    //�ܹ���
    vec3 result = (ambient + diffuse+specular) * objectColor;
    FragColor = vec4(result, 1.0);
}