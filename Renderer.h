//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

namespace PAG {
    class Renderer {
        static Renderer *instancia;
        Renderer();

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();

        void refrescar ();
        void redimensionar(int width, int height);
        void cerrar_ventana(void *ventana, int valor);
        void scroll();

        int get_gladLoadGLLoader(void* procAddr);
        float get_gl_renderer();
        float get_gl_version();
        float get_gl_vendor();
        float get_gl_shading_language_version();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
