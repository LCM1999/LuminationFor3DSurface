#version 450 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec3 Normal;
in vec4 col;

// uniform sampler2D texture_diffuse;

void main() {
    gPosition = FragPos;
    gNormal = normalize(Normal);
    gAlbedoSpec = col;  //texture(texture_diffuse, TexCoords);
}
