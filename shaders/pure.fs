#version 430

in vec3 Normal;
in vec3 FragPos;
// in vec2 TexCoords;
in vec4 col;

struct Light {
    vec3 position;
    vec3 diffuse;
};

out vec4 color;

uniform vec3 highColor;
uniform vec3 lowColor;
uniform Light light;
uniform float ambient;
void main() {
    // diffuse
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0); //cos
    //vec3 diffuse = light.diffuse * diff * high;
    //max((ambient-0.5),0.0)*(highColor-lowColor)
    // + col.rgb
    // color = vec4(max((ambient-0.5),0.0) * col.rgb + diff*light.diffuse*(highColor-col.rgb), 1.0f);
    color = col;
}