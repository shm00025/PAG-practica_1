//
// Created by Santi on 25/11/2026.
//

#ifndef PRACTICA_1_APLICADORLUZFOCO_H
#define PRACTICA_1_APLICADORLUZFOCO_H

#include "AplicadorDeLuz.h"

namespace PAG {
    class AplicadorLuzFoco : public AplicadorDeLuz {
    public:
        void aplicarLuz(PropiedadesLuz pl, Modelo *m) override {
            // Obtenemos el idSP activo desde el modelo. Si implementásemos varios, devolvería una lista
            GLuint idSP = m->getIdSP();

            // La luz en foco cuenta con intensidades difusa y especular, posición, dirección
            std::string nombreUniform = "Id"; // Intensidad difusa
            GLint posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getId() ); }

            nombreUniform = "Is"; // Intensidad especular
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getIs() ); }

            nombreUniform = "posicionLuz"; // Posición de la luz
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getP() ); }

            nombreUniform = "direccionLuz"; // Dirección de la luz
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getD() ); }

            nombreUniform = "direccionLuz"; // Dirección de la luz
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getD() ); }
        }
    };
}

#endif //PRACTICA_1_APLICADORLUZFOCO_H
