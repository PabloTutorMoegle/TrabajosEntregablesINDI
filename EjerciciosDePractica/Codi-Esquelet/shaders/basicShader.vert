#version 330 core

in vec3 vertex;
in vec3 color;
uniform float escalat;
out vec3 fcolor;

void main()  {
    fcolor = color;
    gl_Position = vec4 (vertex * escalat, 1.0);
}
