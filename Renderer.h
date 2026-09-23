//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

namespace PAG {
    class Renderer {
    private:
        static Renderer *instancia;
        Renderer();

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();
        void refrescar ();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
