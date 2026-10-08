//
// Created by Santi on 04/10/2026.
//

#ifndef PRACTICA_1_VENTANAGUI_H
#define PRACTICA_1_VENTANAGUI_H

#include <vector>

#include "../listener.h"
#include "../constantes.h"

namespace PAG {
    class GUI;

    class VentanaGUI {
    protected:
        GUI *padre = nullptr;
        std::vector<Listener*> listeners;

        float tamTexto;

        WindowType tipoVentana;

    public:
        VentanaGUI(const std::vector<Listener*> &listeners) : listeners(listeners), tipoVentana(General) {
            this->tamTexto = 1.0f;
        };
        virtual ~VentanaGUI() = default;

        virtual void dibujar() = 0;

        void addListener(Listener *listener) { listeners.push_back (listener); };
        virtual void warnListeners() = 0;
    };
}


#endif //PRACTICA_1_VENTANAGUI_H
