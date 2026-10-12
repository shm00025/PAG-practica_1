#version 410
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

uniform mat4 matrizMVP;	// Para las coordenadas del vértice
uniform mat4 matrizMV;	// Para los vectores a usar en el futuro como la normal

out vec3 destinationColor;

void main() {
    gl_Position = matrizMVP * vec4(posicion, 1.0);
    destinationColor = color;
}