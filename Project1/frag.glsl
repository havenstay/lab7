#version 460 core
in vec3 Normal;
in vec3 FragPos;
out vec4 frag_colour;
uniform vec4 ourColor;
uniform vec3 ViewPos;

vec3 ambient;
vec3 diffuse;
vec3 specular;

struct Light_st {
vec3 position;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform Light_st light;

struct Material_st {
float shininess;
vec3 ambient;
vec3 diffuse;
vec3 specular;
};

uniform Material_st mat;


void main()
{

//ambient
ambient = light.ambient * mat.ambient;
//diffuse
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position - FragPos);
float diff = max(abs(dot(norm, lightDir)), 0.0);
diffuse = light.diffuse *(diff*mat.ambient);

//specular
vec3 viewDir = normalize(ViewPos-FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
specular = light.specular *(spec*mat.specular);
    vec3 Sum = ambient + diffuse + specular;
    frag_colour = vec4(Sum, 1.0);
}