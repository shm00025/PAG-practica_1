//
// Created by Santi on 25/11/2026.
//

#ifndef PRACTICA_1_APLICADORDELUZ_H
#define PRACTICA_1_APLICADORDELUZ_H

#include "PropiedadesLuz.h"
#include "../Modelo.h"

namespace PAG {
    class AplicadorDeLuz {
    public:
        virtual ~AplicadorDeLuz() = default;

        // Definimos un método virtual puro para que la clase actue como una interfaz

        // Esta función indica al ShaderProgram las subrutinas a activar y los atributos necesarios
        virtual void aplicarLuz(PropiedadesLuz pl, Modelo *m) = 0;
    };
}

#endif //PRACTICA_1_APLICADORDELUZ_H
