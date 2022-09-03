#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform int depthmode;



//…Ó∂»≤‚ ‘
vec4 show(){
    return texture(texture1, TexCoords);
}
vec4 depth(){
    return vec4(vec3(gl_FragCoord.z), 1.0);
}

vec4 LinearizeDepth(){
    float depth = gl_FragCoord.z;
    float near = 0.1; 
    float far  = 100.0;
    float z = depth * 2.0 - 1.0; // back to NDC 
    float result =  (2.0 * near * far) / (far + near - z * (far - near));
    
    return vec4(vec3(result/ far), 1.0);
}

void main()
{    
    if(depthmode==0){
        FragColor = show();
    }else if(depthmode==1){
        FragColor = depth();
    }else if(depthmode==2){
        FragColor = LinearizeDepth();
    }else{
        FragColor = show();
    }
    
}