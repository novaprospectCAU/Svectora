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
uniform float uSpecStrength;

uniform sampler2D uTexture;

void main()
{
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 lightDir = normalize(uLightPos - vFragPos);

    vec3 normal = normalize(vNormal);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = uAmbientStrength * uAmbientColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uSpecStrength);
    vec3 specular = spec * uLightColor;

    vec3 texColor = texture(uTexture, vTexCoord).rgb;

    vec3 result = (ambient + diffuse + specular) * texColor;
    FragColor = vec4(result, 1.0);
}
