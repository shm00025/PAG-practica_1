//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

#include <vector>

#include "listener.h"

/** Este struct encapsula los flags que controlan el funcionamiento de los colores. Si queremos una transición
 *  limpia entre los tres canales, necesitamos que los valores aumenten hasta el máximo y luego disminuyan hasta
 *  el mínimo. Este comportamiento necesita alguna forma de registrar si el valor ahora debe subir o bajar.
 */
struct FlagsOndas {
    bool flags[3] = {true, true, true};
};

namespace PAG {
    class Renderer : public Listener {
        static Renderer *instancia;
        Renderer();
        std::vector<float> colorFondo = {0.6f, 0.6f, 0.6f, 1.0f};

        WindowType tipoVentana;
        std::vector<Listener*> listeners;

        // Render
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();

        void inicializar();
        void refrescar();

        void redimensionar(int width, int height);
        void cerrar_ventana(void *ventana, int valor);
        void scroll(void* flagsOndas, double xoffset, double yoffset);

        void addListener(Listener *listener);
        void warnListeners();
        void wakeUp ( WindowType t, ... ) override;

        void creaShaderProgram();
        void creaModelo();

        int get_gladLoadGLLoader(void* procAddr);
        float get_gl_renderer();
        float get_gl_version();
        float get_gl_vendor();
        float get_gl_shading_language_version();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
