#version 330 core

in vec3 fragNormal;
in vec3 fragPos;
in vec3 fragMatAmb;
in vec3 fragMatDiff;
in vec3 fragMatSpec;
in float fragMatShin;

uniform vec3 ambientLightColor; 
uniform vec3 lightPos; 
uniform vec3 lightColor; 
uniform vec3 farLight1Pos;
uniform vec3 farLight2Pos;
uniform vec3 farLight1Dir;
uniform vec3 farLight2Dir;
uniform vec3 posFocusTorxes[6];
uniform vec3 torchColor;
uniform bool torchOn[6];

out vec4 fragColor;

vec3 WaveNormal(vec2 waveDirNN, float waveLength, vec3 vertex_world, float time){
    float steepness = .5;
    vec2 waveDir = normalize(waveDirNN);
    float wl = 2;
    float f = (2 * 3.14159 / waveLength) * (dot(waveDir, vertex_world.xz) - 0.25 * time);

    vec3 B = vec3(
        1 - (steepness * waveDir.x * waveDir.x * cos(f)),
        waveDir.x * sin(f),
        -steepness * waveDir.x * waveDir.y * cos(f)
    );
    vec3 T = vec3(
        -steepness * waveDir.x * waveDir.y * cos(f),
        waveDir.y * sin(f),
        1 - steepness * waveDir.y * waveDir.y * cos(f)
    );

    return normalize(cross(normalize(T), normalize(B)));
}

// Función para calcular la componente ambiental
vec3 Ambient() {
    return ambientLightColor * fragMatAmb;
}

// Función para calcular la componente difusa
vec3 Diffuse(vec3 norm, vec3 lightDir, vec3 lightColor) {
    float diff = max(dot(norm, lightDir), 0.0);
    return lightColor * fragMatDiff * diff;
}

// Función para calcular la componente especular
vec3 Specular(vec3 norm, vec3 lightDir, vec3 viewDir, vec3 lightColor) {
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fragMatShin);
    return lightColor * fragMatSpec * spec;
}

// Función para calcular la atenuación del spotlight
float SpotLightAttenuation(vec3 lightDir, vec3 spotDir, float cutoff, float exponent) {
    float theta = dot(lightDir, normalize(spotDir));
    float epsilon = cutoff - 0.001; // Pequeña diferencia para suavizar el corte
    return clamp(pow(theta, exponent), 0.0, 1.0);
}

// Función para calcular la atenuación de la antorcha
float TorchAttenuation(float distance) {
    if (distance < 3.0) {
        return 1.0;
    } else {
        return exp(-(distance - 3.0));
    }
}

void main()
{
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos); // La cámara está en (0,0,0)

    vec3 ambient = Ambient();
    vec3 diffuse = Diffuse(norm, lightDir, lightColor);
    vec3 specular = Specular(norm, lightDir, viewDir, lightColor);

    vec3 spotLightDir1 = normalize(farLight1Pos - fragPos);
    float attenuation1 = SpotLightAttenuation(spotLightDir1, farLight1Dir, cos(radians(12.5)), 4.0);
    vec3 spotDiffuse1 = Diffuse(norm, spotLightDir1, lightColor) * attenuation1;
    vec3 spotSpecular1 = Specular(norm, spotLightDir1, viewDir, lightColor) * attenuation1;

    vec3 spotLightDir2 = normalize(farLight2Pos - fragPos);
    float attenuation2 = SpotLightAttenuation(spotLightDir2, farLight2Dir, cos(radians(12.5)), 4.0);
    vec3 spotDiffuse2 = Diffuse(norm, spotLightDir2, lightColor) * attenuation2;
    vec3 spotSpecular2 = Specular(norm, spotLightDir2, viewDir, lightColor) * attenuation2;

    vec3 result = ambient + diffuse + specular + spotDiffuse1 + spotSpecular1 + spotDiffuse2 + spotSpecular2;

    for (int i = 0; i < 6; ++i) {
        if (torchOn[i] == true)
        {
            vec3 torchDir = normalize(posFocusTorxes[i] - fragPos);
            float distance = length(posFocusTorxes[i] - fragPos);
            float torchAttenuation = TorchAttenuation(distance);
            vec3 torchDiffuse = Diffuse(norm, torchDir, torchColor) * torchAttenuation;
            vec3 torchSpecular = Specular(norm, torchDir, viewDir, torchColor) * torchAttenuation;
            result += torchDiffuse + torchSpecular;
        }
    }

    fragColor = vec4(result, 1.0);
}
