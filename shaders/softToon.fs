#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 diffuse;
};

out vec4 color;

uniform Light light;

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far = 10.0;
    float z = depth * 2.0 - 1.0;
    return (2.0 * near) / (far + near - z * (far - near));
}

void main() {
    vec3 textureColor = vec3(0.9,0.9,0.9);
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    //float diff = max(dot(norm, lightDir), 0.0); //cos
    //vec3 diffuse = light.diffuse * diff * textureColor;
    // vec3 toon = (0.5 + 0.5*clamp(dot(norm, lightDir),-1.0,1.0))*textureColor;
    float depth = LinearizeDepth(gl_FragCoord.z);
    //color = vec4(diffuse, 1.0f);
    // color = vec4(toon,1.0f);
    color = vec4(vec3(depth), 1.0f);

}