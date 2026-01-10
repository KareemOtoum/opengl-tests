#version 330 core

layout (location = 0) in vec3 pos;

uniform float time;

void main() {
    float y_offset = sin(pos.x * 3 - time) / 20;

    gl_Position = vec4(pos.x, pos.y + y_offset, pos.z, 1.0);
}