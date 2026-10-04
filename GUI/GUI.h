//
// Created by Santi on 27/09/2026.
//

#ifndef PRACTICA_1_GUI_H
#define PRACTICA_1_GUI_H

#include <iosfwd>
#include <sstream>
#include <vector>
#include <memory>

#include "../listener.h"
#include "VentanaGUI.h"


namespace PAG {
    class GUI {
        static GUI *instancia;
        GUI();

        WindowType tipoVentana;
        std::vector<Listener*> listeners;

        std::vector<std::unique_ptr<VentanaGUI>> ventanas;

    public:
        virtual ~GUI();
        static GUI &getInstancia();

        void poner_linea(std::stringstream &linea);

        void inicializar(void* ventana, Listener* renderer);
        void refrescar();
        void destruir();

        virtual void warnListeners(const char *cadena);
    };
};


#endif //PRACTICA_1_GUI_H
