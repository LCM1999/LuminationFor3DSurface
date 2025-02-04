#version 450 core

// in vec3 Normal;
// in vec3 FragPos;
in vec2 TexCoords;
// in vec4 col;

out vec4 FragColor;

// G-buffer textures
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

struct Light {
    vec3 position;
    vec3 diffuse;
};

// out vec4 color;

uniform vec3 highColor;
uniform vec3 lowColor;
uniform Light light;
uniform vec3 viewPos; // Camera position in world space
uniform float ambientFactor;
// uniform float phi;

//////////////////////////////////////////////////////////////////////////////////////////////////
// RGB colorspace <=> XYZ colorspace <=> LAB colorspace
// vec3 rgbToXyz(vec3 rgb) {
//     // sRGB to linear RGB
//     rgb = pow(rgb, vec3(2.2));

//     // Linear RGB to XYZ using D65 standard illuminant
//     const mat3 rgbToXyzMat = mat3(
//         0.4124564, 0.3575761, 0.1804375,
//         0.2126729, 0.7151522, 0.0721750,
//         0.0193339, 0.1191920, 0.9503041
//     );

//     return rgbToXyzMat * rgb;
// }

// float f(float t) {
//     const float delta = 6.0 / 29.0;
//     if (t > pow(delta, 3.0)) {
//         return pow(t, 1.0 / 3.0);
//     } else {
//         return t / (3.0 * pow(delta, 2.0)) + 4.0 / 29.0;
//     }
// }

// vec3 xyzToLab(vec3 xyz) {
//     // Reference white point for D65
//     const vec3 whitePoint = vec3(0.95047, 1.0, 1.08883);

//     vec3 scaledXyz = xyz / whitePoint;

//     float fx = f(scaledXyz.x);
//     float fy = f(scaledXyz.y);
//     float fz = f(scaledXyz.z);

//     float L = 116.0 * fy - 16.0;
//     float a = 500.0 * (fx - fy);
//     float b = 200.0 * (fy - fz);

//     return vec3(L, a, b);
// }

// float fInverse(float t) {
//     const float delta = 6.0 / 29.0;
//     if (t > delta) {
//         return t * t * t;
//     } else {
//         return 3.0 * pow(delta, 2.0) * (t - 4.0 / 29.0);
//     }
// }

// vec3 labToXyz(vec3 lab) {
//     const vec3 whitePoint = vec3(0.95047, 1.0, 1.08883);

//     float fy = (lab.x + 16.0) / 116.0;
//     float fx = lab.y / 500.0 + fy;
//     float fz = fy - lab.z / 200.0;

//     float x = fInverse(fx);
//     float y = fInverse(fy);
//     float z = fInverse(fz);

//     return vec3(x, y, z) * whitePoint;
// }

// vec3 xyzToRgb(vec3 xyz) {
//     const mat3 xyzToRgbMat = mat3(
//         3.2404542, -1.5371385, -0.4985314,
//         -0.9692660,  1.8760108,  0.0415560,
//         0.0556434, -0.2040259,  1.0572252
//     );

//     vec3 linearRgb = xyzToRgbMat * xyz;

//     // Linear RGB to sRGB
//     return pow(linearRgb, vec3(1.0 / 2.2));
// }
////////////////////////////////////////////////////////////////////

void main() {
    // diffuse
    // vec3 norm = normalize(Normal);
    // vec3 lightPos = light.position;
    // lightPos.x = -lightPos.x;

    // vec3 lightDir = normalize(lightPos - FragPos);
    // float diff = max(dot(norm, -lightDir), 0.0); //cos
    // //vec3 diffuse = light.diffuse * diff * high;
    // //max((ambient-0.5),0.0)*(highColor-lowColor)
    // // + col.rgb
    // // + *(highColor-lowColor)
    // float intensity = -phi * diff;

    // vec3 labCol = xyzToLab(rgbToXyz(col.rgb));
    // labCol.x = clamp(labCol.x + intensity, 0, 100);
    // // color = vec4((max((ambient-0.5),0.0) + diff) * light.diffuse * col.rgb, 1.0f);
    // color = vec4(xyzToRgb(labToXyz(labCol)), 1.0f);

        // Texture coordinates (assume fullscreen quad is rendered)
    // vec2 texCoords = gl_FragCoord.xy / vec2(textureSize(gPosition, 0));

    // Retrieve data from G-buffer
    // vec3 fragPos = texture(gPosition, TexCoords).rgb;
    // vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    // vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
    // float specularStrength = texture(gAlbedoSpec, TexCoords).a;

    // // Calculate lighting
    // vec3 ambient = ambientFactor * albedo;

    // vec3 lightDir = normalize(lightPos - fragPos);
    // float diff = max(dot(normal, lightDir), 0.0);
    // vec3 diffuse = diff * (highColor-lowColor) * albedo;

    // vec3 viewDir = normalize(viewPos - fragPos);
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    // vec3 specular = specularStrength * spec * (highColor-lowColor);

    // vec3 result = ambient + diffuse + specular;
    float depth = texture(gDepth, TexCoords).r;
    FragColor = texture(gAlbedoSpec, TexCoords); //vec4(result, 1.0);
    // vec4(vec3(texture(gDepth, TexCoords).r), 1.0);
}