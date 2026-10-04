#version 410
in vec3 destinationColor;

out vec4 colorFragmento;

void main() {
    colorFragmento = vec4(destinationColor, 1.0);
}