//
// Created by Santi on 12/10/2026.
//

#ifndef PRACTICA_1_VENTANACAMARAGUI_H
#define PRACTICA_1_VENTANACAMARAGUI_H

#include "VentanaGUI.h"


namespace PAG {
    class VentanaCamaraGUI : public VentanaGUI, public Listener {

    public:
        VentanaCamaraGUI(const std::vector<Listener*> &listeners);
        ~VentanaCamaraGUI() override = default;

        void dibujar() override;

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANACAMARAGUI_H
