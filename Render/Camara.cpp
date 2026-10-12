//
// Created by Santi on 12/10/2026.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "Camara.h"

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

    // Eje v o x
    void Camara::pan(GLfloat angulo) {
        glm::vec3 direccion; // Calculamos la nueva dirección

        direccion.x = (float) cos(glm::radians(angulo));
        direccion.y = 0.0f;
        direccion.z = (float) sin(glm::radians(angulo));

        glm::vec3 frente = glm::normalize(direccion);

        this-> v = glm::lookAt(position, position + frente,glm::vec3(0.0f, 1.0f, 0.0f));
        recalcularMatrices(); // Actualizamos la matriz compuesta de proyección y visión para vértices
    }

    // Eje u o y
    void Camara::tilt(GLfloat anguloRadianes) {}

    void Camara::dolly(GLfloat espacioX, GLfloat espacioZ) {}

    void Camara::crane(GLfloat espacioY) {}

    void Camara::orbit(GLfloat anguloLatitude, GLfloat anguloLongitud) {}

    void Camara::zoom(GLfloat angulo) {}
}