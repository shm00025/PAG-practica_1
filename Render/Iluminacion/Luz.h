//
// Created by Santi on 16/11/2026.
//

#ifndef PRACTICA_1_LUZ_H
#define PRACTICA_1_LUZ_H

#include <map>
#include <memory>

#include "../../constantes.h"
#include "PropiedadesLuz.h"
#include "../Modelo.h"
#include "AplicadorDeLuz.h"

namespace PAG {
    class Luz {
        TipoLuz tipo;
        PropiedadesLuz propiedades;
        std::map<TipoLuz, std::unique_ptr<AplicadorDeLuz>> aplicadores;

        bool encendida = true;

    public:
        Luz();
        ~Luz() = default;

        void aplicarLuz(Modelo *m);
        void cambiarEstado() { this->encendida = !this->encendida; }

        void actualizarLuz(PropiedadesLuz pl) { this->propiedades = pl; }
    };
}


#endif //PRACTICA_1_LUZ_H
