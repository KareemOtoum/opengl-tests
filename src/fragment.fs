#version 330 core

in vec3 ourcol;
out vec4 fragcol;

uniform sampler2D ourTexture;
in vec2 texcoord;

void main() {
    fragcol = texture(ourTexture, texcoord);
}