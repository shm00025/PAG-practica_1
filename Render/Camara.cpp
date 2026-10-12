//
// Created by Santi on 12/10/2026.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <iostream>

#include "Camara.h"

#include "../constantes.h"

namespace PAG {
    Camara::Camara() : v(), p(), pv(), w(0), h(0), aspectRatio(0) {
        this->v = glm::lookAt(position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void Camara::recalcularMatrices() {
        // Creamos la matriz con la proyección y la visión para los vértices
        this->pv = this->p * this->v;
    }

    void Camara::setWH(GLint w, GLint h) {
        this->w = w;
        this->h = h;
        this->aspectRatio = ((GLfloat) w / (GLfloat) h);

        this->p = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->zNear, this->zFar);
        recalcularMatrices(); // Actualizamos la matriz compuesta de proyección y visión para vértices
    }

    void Camara::movimientoPanTilt() {
        glm::vec3 direccion; // Calculamos la nueva dirección

        direccion.x = (float) cos(glm::radians(this->anguloPan)) * (float) cos(glm::radians(this->anguloTilt));
        direccion.y = (float) sin(glm::radians(this->anguloTilt));
        direccion.z = (float) sin(glm::radians(this->anguloPan)) * (float) cos(glm::radians(this->anguloTilt));

        glm::vec3 frente = glm::normalize(direccion);

        this->v = glm::lookAt(position, position + frente,glm::vec3(0.0f, 1.0f, 0.0f));
        recalcularMatrices(); // Actualizamos la matriz compuesta de proyección y visión para vértices
    }

    // Eje v o y
    void Camara::pan(GLfloat angulo) {
        this->anguloPan += angulo; // Actualizamos el ángulo
        movimientoPanTilt();
    }

    // Eje u o x
    void Camara::tilt(GLfloat angulo) {
        this->anguloTilt += angulo; // Actualizamos el ángulo
        if (this->anguloTilt > limiteTilt) this->anguloTilt = limiteTilt;
        if (this->anguloTilt < (-1) * limiteTilt) this->anguloTilt = (-1) * limiteTilt;

        movimientoPanTilt();
    }

    void Camara::dolly(GLfloat espacioX, GLfloat espacioZ) {
        this->necesarioRecalcularRadio = true;

        // Calculamos los desplazamientos
        this->position.x += espacioX;
        this->position.z += espacioZ;
        this->lookAt.x += espacioX;
        this->lookAt.z += espacioZ;

        this->v = glm::lookAt(this->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        recalcularMatrices();
    }

    void Camara::crane(GLfloat espacioY) {
        this->necesarioRecalcularRadio = true;

        // Calculamos el desplazamiento
        this->position.y += espacioY;
        this->lookAt.y += espacioY;

        this->v = glm::lookAt(this->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
        recalcularMatrices();
    }

    void Camara::orbit(GLfloat anguloLatitudP, GLfloat anguloLongitudP) {
        // Actualizamos los ángulos de rotación y limitamos el vertical
        this->anguloLatitud += anguloLatitudP;
        this->anguloLongitud += anguloLongitudP;
        if (this->anguloLongitud > limiteTilt) this->anguloLongitud = limiteTilt;
        if (this->anguloLongitud < (-1) * limiteTilt) this->anguloLongitud = (-1) * limiteTilt;

        if (necesarioRecalcularRadio) {
            // Obtenemos la distancia de la cámara al centro de coordenadas
            glm::vec3 diferencia = - position;
            this->radioOrbita = sqrt((
                (diferencia.x * diferencia.x) +
                (diferencia.y * diferencia.y) +
                (diferencia.z * diferencia.z)));
            this->necesarioRecalcularRadio = false;
        }

        // Actualizamos las posiciones de la cámara dado que es esta la que rota
        this->position.x = radioOrbita * (float) cos(glm::radians(this->anguloLatitud)) * (float) cos(glm::radians(this->anguloLongitud));
        this->position.y = radioOrbita * (float) sin(glm::radians(this->anguloLongitud));
        this->position.z = radioOrbita * (float) sin(glm::radians(this->anguloLatitud)) * (float) cos(glm::radians(this->anguloLongitud));

        this->v = glm::lookAt(this->position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        recalcularMatrices();
    }

    void Camara::zoom(GLfloat angulo) {
        this->fovY += angulo;
        if (this->fovY < minFovY) this->fovY = minFovY;
        if (this->fovY > maxFovY) this->fovY = maxFovY;

        this->p = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->zNear, this->zFar);
        recalcularMatrices(); // Actualizamos la matriz compuesta de proyección y visión para vértices
    }

    void Camara::movimiento(int tipoMovimiento, float valorX, float valorY) {
        switch (tipoMovimiento) {
            case 0: { // Pan
                this->pan(valorX * factorConversionAngulo);
                break;
            }
            case 1: { // Tilt
                this->tilt(valorY * factorConversionAngulo);
                break;
            }
            case 2: { // Dolly
                this->dolly(valorX * factorConversionDesplazamiento, valorY * factorConversionDesplazamiento);
                break;
            }
            case 3: { // Crane
                this->crane(valorY * factorConversionDesplazamiento);
                break;
            }
            case 4: { // Orbit
                this->orbit(valorX * factorConversionAngulo, valorY * factorConversionAngulo);
                break;
            }
            case 5: { // Zoom
                this->zoom(valorY * factorConversionAngulo);
                break;
            }
        }
    }
}
