//
// Created by Santi on 27/09/2026.
//

#ifndef PRACTICA_1_GUI_H
#define PRACTICA_1_GUI_H

#include <iosfwd>
#include <sstream>
#include <vector>

#include "listener.h"


constexpr int r = 0, g = 1, b = 2, alfa = 3;
constexpr int numCanales = 3;

namespace PAG {
    class GUI {
        static GUI *instancia;
        GUI();

        std::stringstream Items;
        void ClearLog();

        std::vector<Listener*> listeners;

        bool necesarioPintar = true;
        std::vector<float> fondo = {0.0f, 0.0f, 0.0f, 0.0f};

    public:
        virtual ~GUI();
        static GUI &getInstancia();

        void poner_linea(std::stringstream &linea);

        void addListener(Listener *listener);
        void warnListeners();

        void inicializar(void* ventana);
        void refrescar();
        void destruir();

        void pintar_ventana_color();
        void pintar_consola();
    };
};


#endif //PRACTICA_1_GUI_H
