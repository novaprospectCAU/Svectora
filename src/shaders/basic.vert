#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vColor;
out vec3 vNormal;
out vec3 vFragPos;

void main() {
  vColor = aColor;
  vNormal = mat3(transpose(inverse(uModel))) * aNormal;

  vec4 worldPos = uModel * vec4(aPos, 1.0);
  vFragPos = worldPos.xyz;

  gl_Position = uProj * uView * worldPos;
}