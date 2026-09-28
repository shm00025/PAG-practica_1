//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

#include <vector>

#include "listener.h"

namespace PAG {
    class Renderer : public Listener {
        static Renderer *instancia;
        Renderer();
        std::vector<float> colorFondo = {0.6f, 0.6f, 0.6f, 1.0f};

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();

        void inicializar();
        void refrescar();
        void redimensionar(int width, int height);
        void cerrar_ventana(void *ventana, int valor);
        void scroll();

        void wakeUp ( WindowType t, ... ) override;


        int get_gladLoadGLLoader(void* procAddr);
        float get_gl_renderer();
        float get_gl_version();
        float get_gl_vendor();
        float get_gl_shading_language_version();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
