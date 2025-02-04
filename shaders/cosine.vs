#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// layout (location = 1) in vec3 aNormal;
// layout (location = 2) in float aScalar;

// out vec3 Normal;
// out vec3 FragPos;
out vec2 TexCoords;
// out vec4 col;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

// float colormap_red(float x) {
//     if (x < 0.7) {
//         return 4.0 * x - 1.5;
//     } else {
//         return -4.0 * x + 4.5;
//     }
// }

// float colormap_green(float x) {
//     if (x < 0.5) {
//         return 4.0 * x - 0.5;
//     } else {
//         return -4.0 * x + 3.5;
//     }
// }

// float colormap_blue(float x) {
//     if (x < 0.3) {
//        return 4.0 * x + 0.5;
//     } else {
//        return -4.0 * x + 2.5;
//     }
// }

// vec4 colormap(float x) {
//     float r = clamp(colormap_red(x), 0.0, 1.0);
//     float g = clamp(colormap_green(x), 0.0, 1.0);
//     float b = clamp(colormap_blue(x), 0.0, 1.0);
//     return vec4(r, g, b, 1.0);
// }

void main() {
    gl_Position = vec4(aPos, 1.0);  
    //projection * view *  model * vec4(aPos, 1.0f);
    // FragPos = vec3(model * vec4(aPos, 1.0f)); 
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    // col = colormap(aScalar);
}

