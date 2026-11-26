//
// Created by Santi on 16/11/2026.
//

#include "Luz.h"

#include "AplicadorLuzAmbiente.h"
#include "AplicadorLuzDireccional.h"
#include "AplicadorLuzFoco.h"
#include "AplicadorLuzPuntual.h"

namespace PAG {
    Luz::Luz() : aplicadores(), tipo(Ambiente) {
        // Rellenamos el mapa con instancias de cada tipo de aplicador
        this->aplicadores[Ambiente] = std::make_unique<AplicadorLuzAmbiente>();
        this->aplicadores[Puntual] = std::make_unique<AplicadorLuzPuntual>();
        this->aplicadores[Direccional] = std::make_unique<AplicadorLuzDireccional>();
        this->aplicadores[Foco] = std::make_unique<AplicadorLuzFoco>();
    }

    void Luz::aplicarLuz(Modelo *m) {
        if (!this->encendida) return;

        // Invocamos la luz que tenemos activa para el dibujado
        this->aplicadores[tipo]->aplicarLuz(this->propiedades, m);
    }
}