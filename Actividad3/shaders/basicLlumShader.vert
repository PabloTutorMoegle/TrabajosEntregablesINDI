#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 fragMatAmb;
out vec3 fragMatDiff;
out vec3 fragMatSpec;
out float fragMatShin;

void main()
{	
    fragNormal = mat3(transpose(inverse(TG))) * normal; // Transform normal to world space
    fragPos = vec3(TG * vec4(vertex, 1.0)); // Vertex position in world space
    fragMatAmb = matamb;
    fragMatDiff = matdiff;
    fragMatSpec = matspec;
    fragMatShin = matshin;
    
    gl_Position = proj * view * TG * vec4(vertex, 1.0);
}
