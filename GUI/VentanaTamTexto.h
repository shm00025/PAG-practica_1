//
// Created by Santi on 05/10/2026.
//

#ifndef PRACTICA_1_VENTANATAMTEXTO_H
#define PRACTICA_1_VENTANATAMTEXTO_H

#include "VentanaGUI.h"


namespace PAG {
    class VentanaTamTexto : public VentanaGUI {
    public:
        VentanaTamTexto(const std::vector<Listener*> &listeners);
        ~VentanaTamTexto() override = default;

        void dibujar() override;

        void warnListeners() override;
    };
}


#endif //PRACTICA_1_VENTANATAMTEXTO_H
