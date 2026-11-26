//
// Created by Santi on 25/11/2026.
//

#ifndef PRACTICA_1_APLICADORLUZAMBIENTE_H
#define PRACTICA_1_APLICADORLUZAMBIENTE_H

#include "AplicadorDeLuz.h"

namespace PAG {
    class AplicadorLuzAmbiente : public AplicadorDeLuz {
    public:
        void aplicarLuz(PropiedadesLuz pl, Modelo *m) override {
            // Obtenemos el idSP activo desde el modelo. Si implementásemos varios, devolvería una lista
            GLuint idSP = m->getIdSP();

            // La luz ambiente solo necesita la intensidad ambiente
            std::string nombreUniform = "Ia"; // Intensidad ambiente
            GLint posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if ( posicion != -1 ) { glUniform3fv ( posicion, 1, pl.getIa() ); }
        }
    };
}

#endif //PRACTICA_1_APLICADORLUZAMBIENTE_H
