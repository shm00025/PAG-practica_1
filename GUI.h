//
// Created by Santi on 27/09/2026.
//

#ifndef PRACTICA_1_GUI_H
#define PRACTICA_1_GUI_H


constexpr int r = 0, g = 1, b = 2, alfa = 3;
constexpr int numCanales = 3;

namespace PAG {
    class GUI {
        static GUI *instancia;
        GUI();

    public:
        virtual ~GUI();
        static GUI &getInstancia();

        void inicializar(void* ventana);
        void refrescar();
        void destruir();

        void pintar_ventana_color();
    };
};


#endif //PRACTICA_1_GUI_H
