//
// Created by Santi on 12/10/2026.
//

#ifndef PRACTICA_1_CAMARA_H
#define PRACTICA_1_CAMARA_H

#include <glm/glm.hpp>


namespace PAG {
    class Camara {
        float anguloPan = -90;
        float anguloTilt = 0;

        glm::vec3 traslaciones = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 rotaciones = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::mat4 v, p, pv;
        int w, h;
        float aspectRatio;
        float zNear = 0.1f, zFar = 10.0f;
        float fovY = 45.0f;

        void recalcularMatrices();
        void movimientoPanTilt();

        void pan(float angulo); // Eje v o x
        void tilt(float angulo); // Eje u o y
        void dolly(float espacioX, float espacioZ);
        void crane(float espacioY);
        void orbit(float anguloLatitude, float anguloLongitud);
        void zoom(float angulo);

    public:
        Camara();
        ~Camara() = default;

        void setWH(int w, int h);
        glm::mat4 getV() { return this->v; }
        glm::mat4 getPV() { return this->pv; }

        void movimiento(int tipoMovimiento, float valorX, float valorY);
    };
}


#endif //PRACTICA_1_CAMARA_H
