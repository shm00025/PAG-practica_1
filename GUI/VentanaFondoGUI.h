//
// Created by Santi on 04/10/2026.
//

#ifndef PRACTICA_1_VENTANAFONDOGUI_H
#define PRACTICA_1_VENTANAFONDOGUI_H

#include "VentanaGUI.h"


namespace PAG {
    class VentanaFondoGUI : public VentanaGUI {
        bool necesarioPintar = true;
        std::vector<float> fondo = {0.0f, 0.0f, 0.0f, 0.0f};

    public:
        VentanaFondoGUI(const std::vector<Listener*> &listeners);
        ~VentanaFondoGUI() override = default;

        void dibujar() override;

        void warnListeners() override;
    };
}


#endif //PRACTICA_1_VENTANAFONDOGUI_H
