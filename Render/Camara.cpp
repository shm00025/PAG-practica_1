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

    void Camara::dolly(GLfloat espacioX, GLfloat espacioZ) {}

    void Camara::crane(GLfloat espacioY) {}

    void Camara::orbit(GLfloat anguloLatitude, GLfloat anguloLongitud) {

    }

    void Camara::zoom(GLfloat angulo) {}



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
                break;
            }
            case 3: { // Crane
                break;
            }
            case 4: { // Orbit
                break;
            }
            case 5: { // Zoom
                break;
            }
        }
    }
}
