#version 330 core

layout (location = 0) in vec3 pos;

uniform float time;

void main() {
    float y_offset = sin(pos.x * 20 - (time * 3)) / 20;
    y_offset += sin(pos.x * 4 + (time * 3)) / 30;
    gl_Position = vec4(pos.x, pos.y - y_offset, pos.z, 1.0);
}