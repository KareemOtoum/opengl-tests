#version 330 core

in vec3 ourcol;
out vec4 fragcol;

void main() {
    fragcol = vec4(ourcol, 1.0);
}