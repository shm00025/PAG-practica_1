#version 410
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 matrizMVP;	// Para las coordenadas del vértice
uniform mat4 matrizMV;	// Para los vectores a usar en el futuro como la normal

out vec3 position;
out vec3 normal;

void main() {
    normal = vec3(matrizMV * vec4(vNormal, 0.0));
    position = vec3(matrizMV * vec4(vPosition, 1.0));
    gl_Position = matrizMVP * vec4(vPosition, 1.0);
}