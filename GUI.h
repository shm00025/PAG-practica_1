//
// Created by Santi on 27/09/2026.
//

#ifndef PRACTICA_1_GUI_H
#define PRACTICA_1_GUI_H


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
    };
};


#endif //PRACTICA_1_GUI_H
