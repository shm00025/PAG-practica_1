#version 410
in vec3 position;
in vec3 normal;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float eS;

out vec4 colorFragmento;

subroutine vec4 fCalcularColor();
subroutine uniform fCalcularColor uMetodoColorElegido;

subroutine(fCalcularColor)
vec4 solido() {
    // Para el modo solido usamos el cálculo de siempre con la normal
    return vec4(kd, 1.0);
}

subroutine(fCalcularColor)
vec4 alambre() {
    // Para el modo alambre devolvemos directamente el color rojo
    return vec4(1, 0, 0, 1);
}

void main() {
    colorFragmento = uMetodoColorElegido();
}