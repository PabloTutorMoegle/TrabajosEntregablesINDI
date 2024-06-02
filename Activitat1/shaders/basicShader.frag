#version 330 core

out vec4 FragColor;
in vec4 vertex_color;

uniform vec3 nuvolPos[5];

void main() {

    FragColor = vertex_color;

}

