#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vFragPos;

in vec2 vTexCoord;

out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

uniform float uAmbientStrength;
uniform vec3 uAmbientColor;

uniform sampler2D uTexture;

uniform float uRoughness;
uniform float uK_d;
uniform float uK_s;
uniform vec3 uF0;

const float PI = 3.14159265359;
const float EPS = 1e-5;

float D_GGX(vec3 normal, vec3 halfVector, float alpha)
{
    float a2 = alpha * alpha;
    float NdotH = max(dot(normal, halfVector), 0.0);
    float NdotH2 = NdotH * NdotH;
    float divider = NdotH2 * (a2 - 1.0) + 1.0;
    float divider2 = divider * divider;
    return a2/ (PI * divider2 + EPS);
}

vec3 F_Schlick(vec3 uF0, vec3 viewDir, vec3 halfVector)
{
    float multiplier = 1.0 - max(dot(viewDir, halfVector), 0.0);
    multiplier *= multiplier * multiplier * multiplier * multiplier;
    return uF0 + (vec3(1.0) - uF0) * multiplier;
}

float G_Smith(vec3 normal, vec3 viewDir, vec3 lightDir, float uRoughness)
{
    float k = (uRoughness + 1.0) * (uRoughness + 1.0) / 8;
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = max(dot(normal, lightDir), 0.0);
    float g0 = NdotV / (NdotV * (1.0 - k) + k);
    float g1 = NdotL / (NdotL * (1.0 - k) + k);
    return g0 * g1;
}

void main()
{
    float alpha = max(uRoughness * uRoughness, EPS);

    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    vec3 normal = normalize(vNormal);

    vec3 halfVector = normalize(lightDir + viewDir);

    float D = D_GGX(normal, halfVector, alpha);
    vec3 F = F_Schlick(uF0, viewDir, halfVector);
    float G = G_Smith(normal, viewDir, lightDir, uRoughness);

    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 specular = uK_s * (D * G) / max(4.0 * NdotV * NdotL, EPS) * F;

    vec3 albedo = texture(uTexture, vTexCoord).rgb;
    vec3 kd = (vec3(1.0) - F);

    vec3 diffuse = uK_d * kd * (albedo / PI);

    vec3 Lo = (diffuse + specular) * uLightColor * NdotL;
    vec3 ambient = uAmbientStrength * uAmbientColor * albedo;
    vec3 color = ambient + Lo;
    FragColor = vec4(color, 1.0);
}

