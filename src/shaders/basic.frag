#version 330 core

in vec3 vColor;
in vec3 vNormal;
in vec3 vFragPos;

out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uViewPos;

void main()
{
    vec3 lightDir = normalize(uLightPos - vFragPos);

    vec3 normal = normalize(vNormal);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    vec3 ambient = 0.1 * uLightColor;

    vec3 result = (ambient + diffuse) * vColor;
    FragColor = vec4(result, 1.0);
}
