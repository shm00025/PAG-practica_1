//
// Created by Santi on 04/10/2026.
//

#ifndef PRACTICA_1_VENTANACONSOLAGUI_H
#define PRACTICA_1_VENTANACONSOLAGUI_H

#include <sstream>

#include "VentanaGUI.h"


namespace PAG {
    class VentanaConsolaGUI : public VentanaGUI, public Listener {
        std::stringstream Items;
        void ClearLog();

    public:
        VentanaConsolaGUI(const std::vector<Listener*> &listeners);
        ~VentanaConsolaGUI() override = default;

        void dibujar() override;

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANACONSOLAGUI_H
