#version 410
in vec3 position;
in vec3 normal;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float eS;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;

uniform vec3 posicionLuz; // En espacio de visión
uniform vec3 direccionLuz; // En espacio de visión y normalizado
uniform float anguloFoco;

out vec4 colorFragmento;

subroutine vec3 fTipoLuz();
subroutine uniform fTipoLuz uTipoLuz;

subroutine(fTipoLuz)
vec3 basico() {
    // Versión básica de la práctica 7
    return kd;
}

subroutine(fTipoLuz)
vec3 ambiente() {
    // No afectan posiciones ni normales
    return vec3(Ia * ka);
}

subroutine(fTipoLuz)
vec3 puntual() {
    vec3 n = normalize(normal);

    vec3 l = normalize(posicionLuz - position); // Vector hacia la luz
    vec3 v = normalize(-position); // Vector a la cámara
    vec3 r = reflect(-l, n); // Refleja el rayo de luz con respecto a la normal

    // Para la luz difusa calculamos (l x n) para ver si la luz ilumina la zona
    vec3 difusa = (Id * kd * max(dot(l, n), 0.0));

    // Para la luz especular calculamos (r x v) para ver si la cámara ve el reflejo
    // A nosotros nos llega el reflejo siguiendo el rayo r
    vec3 especular = (Is * ks * pow(max(dot(r, v), 0.0), eS));

    return difusa + especular;
}

subroutine(fTipoLuz)
vec3 direccional() {
    vec3 n = normalize(normal);

    vec3 l = -direccionLuz; // Direccion de la luz
    vec3 v = normalize(-position); // Vector a la cámara
    vec3 r = reflect(-l, n); // Refleja el rayo de luz con respecto a la normal

    // Para la luz difusa calculamos (l x n) para ver si la luz ilumina la zona
    vec3 difusa = (Id * kd * max(dot(l, n), 0.0));

    // Para la luz especular calculamos (r x v) para ver si la cámara ve el reflejo
    // A nosotros nos llega el reflejo siguiendo el rayo r
    vec3 especular = (Is * ks * pow(max(dot(r, v), 0.0), eS));

    return difusa + especular;
}

subroutine(fTipoLuz)
vec3 foco() {
    vec3 l = normalize(posicionLuz - position); // Vector hacia la luz
    vec3 d = direccionLuz;
    float cosGamma = cos(radians(anguloFoco));
    float factorFoco = 1.0;

    // Estudiamos si el objeto está dentro del cono del foco
    // Si el coseno del ángulo con el eje de la luz es menor o igual
    // que el de apertura, está iluminado
    if (dot(-l, d) < cosGamma) {factorFoco = 0.0;}

    vec3 n = normalize(normal);
    vec3 v = normalize(-position); // Vector a la cámara
    vec3 r = reflect(-l, n); // Refleja el rayo de luz con respecto a la normal

    // Para la luz difusa calculamos (l x n) para ver si la luz ilumina la zona
    vec3 difusa = (Id * kd * max(dot(l, n), 0.0));

    // Para la luz especular calculamos (r x v) para ver si la cámara ve el reflejo
    // A nosotros nos llega el reflejo siguiendo el rayo r
    vec3 especular = (Is * ks * pow(max(dot(r, v), 0.0), eS));

    return factorFoco * (difusa + especular);
}

void main() {
    colorFragmento = vec4(uTipoLuz(), 1.0);
}