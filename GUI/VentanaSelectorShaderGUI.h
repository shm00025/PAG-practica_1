//
// Created by Santi on 05/10/2026.
//

#ifndef PRACTICA_1_VENTANASELECTORSHADERGUI_H
#define PRACTICA_1_VENTANASELECTORSHADERGUI_H

#include <string>

#include "VentanaGUI.h"


namespace PAG {
    class VentanaSelectorShaderGUI : public VentanaGUI, public Listener {
        std::string nombre;

    public:
        VentanaSelectorShaderGUI(const std::vector<Listener*> &listeners);
        ~VentanaSelectorShaderGUI() override = default;

        void dibujar() override;

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANASELECTORSHADERGUI_H
