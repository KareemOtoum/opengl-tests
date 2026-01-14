#version 330 core

out vec4 fragcol;

uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 result = ambient * objectColor;
  fragcol = vec4(result, 1.);
}