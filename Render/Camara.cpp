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
        // Recalculamos la matriz de visión y las demás
        this->v = glm::lookAt(this->position, this->position + this->lookAt,glm::vec3(0.0f, 1.0f, 0.0f));
        this->pv = this->p * this->v; // Creamos la matriz con la proyección y la visión para los vértices
    }

    void Camara::setWH(GLint w, GLint h) {
        this->w = w;
        this->h = h;
        this->aspectRatio = ((GLfloat) w / (GLfloat) h);

        this->p = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->zNear, this->zFar);
        recalcularMatrices(); // Actualizamos la matriz compuesta de proyección y visión para vértices
    }

    // Eje v o y
    void Camara::pan(GLfloat angulo) {
        // Generamos la matriz de rotación
        glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
        // Calculamos la rotación del vector de dirección. Debemos transformar las coordenadas a vectores de 4 para que se pueda operar
        // con las matrices de transformaciones, luego simplemente se recorta la última componente.
        // Hay que normalizar el dato resultante para que el resto de operaciones se lleven a cabo correctamente, como los productos
        lookAt = glm::normalize(glm::vec3(rotacion * glm::vec4(lookAt, 0.0f))); // Rellenamos con 0.0f porque es un vector

        recalcularMatrices(); // Recalculamos la matriz de visión y las demás
    }

    // Eje u o x
    void Camara::tilt(GLfloat angulo) {
        // Generamos la matriz de rotación
        glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(angulo), glm::vec3(1.0f, 0.0f, 0.0f));
        // Calculamos la rotación del vector de dirección. Debemos transformar las coordenadas a vectores de 4 para que se pueda operar
        // con las matrices de transformaciones, luego simplemente se recorta la última componente.
        // Hay que normalizar el dato resultante para que el resto de operaciones se lleven a cabo correctamente, como los productos
        glm::vec3 lookAtRotado = glm::normalize(glm::vec3(rotacion * glm::vec4(lookAt, 0.0f))); // Rellenamos con 0.0f porque es un vector

        // Comprobamos si estamos mirando hacia arriba
        float producto = glm::dot(lookAtRotado, glm::vec3(0.0f, 1.0f, 0.0f));
        if (producto < epsilonProducto && producto > -epsilonProducto) { // Solo aplicamos el movimiento si los vectores no son paralelos, esto es, no miramos hacia arriba o abajo
            this->lookAt = lookAtRotado; // Actualizamos el vector dirección

            recalcularMatrices(); // Recalculamos la matriz de visión y las demás
        }
    }

    void Camara::dolly(GLfloat espacioX, GLfloat espacioZ) {
        // Calculamos la matriz de traslación
        glm::mat4 traslacion = glm::translate(glm::mat4(1.0f), glm::vec3(espacioX, 0.0f, espacioZ));
        // Calculamos la traslación de la posición de la cámara. Debemos transformar las coordenadas a vectores de 4 para que se pueda operar
        // con las matrices de transformaciones, luego simplemente se recorta la última componente.
        position = glm::vec3(traslacion * glm::vec4(position, 1.0f)); // Rellenamos con 1.0f porque son coordenadas

        recalcularMatrices(); // Recalculamos la matriz de visión y las demás
    }

    void Camara::crane(GLfloat espacioY) {
        // Calculamos la matriz de traslación
        glm::mat4 traslacion = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, espacioY, 0.0f));
        // Calculamos la traslación de la posición de la cámara. Debemos transformar las coordenadas a vectores de 4 para que se pueda operar
        // con las matrices de transformaciones, luego simplemente se recorta la última componente.
        position = glm::vec3(traslacion * glm::vec4(position, 1.0f)); // Rellenamos con 1.0f porque son coordenadas

        recalcularMatrices(); // Recalculamos la matriz de visión y las demás
    }

    void Camara::orbit(GLfloat anguloLatitudP, GLfloat anguloLongitudP) {
        // Obtenemos los vectores de la cámara
        glm::vec3 n = glm::normalize(-position); // Normalizamos e invertimos el vector que mira al centro
        glm::vec3 u = glm::normalize(glm::cross(n, glm::vec3(0.0f, 1.0f, 0.0f))); // Obtenemos u como n * Y
        glm::vec3 v = glm::normalize(glm::cross(n, u)); // Obtenemos v como n * u

        // Generamos la matriz de rotación en latitud
        glm::mat4 rotacionLat = glm::rotate(glm::mat4(1.0f), glm::radians(anguloLatitudP), v);
        // Calculamos la rotación de la posición de la cámara. Debemos transformar las coordenadas a vectores de 4 para que se pueda operar
        // con las matrices de transformaciones, luego simplemente se recorta la última componente.
        position = glm::vec3(rotacionLat * glm::vec4(position, 1.0f)); // Rellenamos con 1.0f porque son coordenadas

        // Obtenemos el vector u de la cámara para rotar con respecto a este
        glm::vec3 vectorDireccionPreLongitud = glm::normalize(-position);
        u = glm::normalize(glm::cross(vectorDireccionPreLongitud, glm::vec3(0.0f, 1.0f, 0.0f)));

        // Calculamos la rotación de longitud
        glm::mat4 rotacionLon = glm::rotate(glm::mat4(1.0f), glm::radians(anguloLongitudP), u);
        glm::vec3 posicionRotada = glm::vec3(rotacionLon * glm::vec4(position, 1.0f));

        // Obtenemos el vector entre el origen de coordenadas y la cámara (Posición en negativo) 0 - a = -a
        glm::vec3 vectorDireccionPostLongitud = glm::normalize(-posicionRotada);
        // Comprobamos si estamos mirando hacia arriba
        float producto = glm::dot(vectorDireccionPostLongitud, glm::vec3(0.0f, 1.0f, 0.0f));
        if (producto < epsilonProducto && producto > -epsilonProducto) { // Solo aplicamos el movimiento si los vectores no son paralelos, esto es, no miramos hacia arriba o abajo
            this->position = posicionRotada; // Actualizamos las posiciones
        }

        lookAt = glm::normalize(-position);
        recalcularMatrices(); // Recalculamos la matriz de visión y las demás
    }

    void Camara::zoom(GLfloat angulo) {
        this->fovY += angulo;
        if (this->fovY < minFovY) this->fovY = minFovY;
        if (this->fovY > maxFovY) this->fovY = maxFovY;

        this->p = glm::perspective(glm::radians(this->fovY), this->aspectRatio, this->zNear, this->zFar);
        recalcularMatrices(); // Recalculamos la matriz de visión y las demás
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
                this->orbit(valorX * factorConversionOrbita, valorY * factorConversionOrbita);
                break;
            }
            case 5: { // Zoom
                this->zoom(valorY * factorConversionAngulo);
                break;
            }
        }
    }
}
