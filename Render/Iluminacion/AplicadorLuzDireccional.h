//
// Created by Santi on 25/11/2026.
//

#ifndef PRACTICA_1_APLICADORLUZDIRECCIONAL_H
#define PRACTICA_1_APLICADORLUZDIRECCIONAL_H

#include "AplicadorDeLuz.h"

namespace PAG {
    class AplicadorLuzDireccional : public AplicadorDeLuz {
    public:
        void aplicarLuz(PropiedadesLuz pl, Modelo *m) override {
            // Obtenemos el idSP activo desde el modelo. Si implementásemos varios, devolvería una lista
            GLuint idSP = m->getIdSP();

            // La luz direccional cuenta con intensidades difusa y especular y dirección
            std::string nombreUniform = "Id"; // Intensidad difusa
            GLint posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getId() ); }

            nombreUniform = "Is"; // Intensidad especular
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getIs() ); }

            nombreUniform = "anguloFoco"; // Dirección de la luz
            posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform1f ( posicion, pl.getGamma() ); }
        }
    };
}

#endif //PRACTICA_1_APLICADORLUZDIRECCIONAL_H
