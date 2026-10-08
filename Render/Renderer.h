//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

#include <vector>
#include <map>

#include "ShaderProgram.h"
#include "../listener.h"

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
        std::vector<float> colorFondo = {0.0f, 0.0f, 0.0f, 1.0f};

        WindowType tipoVentana;
        std::map<WindowType, std::vector<Listener*>> listeners;

        ShaderProgram shaderProgram;
        std::string mensajeError = "";

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();

        void inicializar();
        void refrescar();

        void redimensionar(int width, int height);
        void cerrar_ventana(void *ventana, int valor);
        void scroll(void* flagsOndas, double xoffset, double yoffset);

        /* Quiero avisar al fondo del cambio de color con la rueda del ratón, pero también a la
         * consola cuando se de un error. Mi propuesta ha sido indicar el tipo de ventana a la
         * que se va a comunicar, para enviar un dato u otro. El problema está en que todos los
         * listeners recibirán el dato. Una posible solución es usar un mapa de listas de listeners
         * con claves en el enumerado de tipos de ventana, para solo avisar a los listeners de ese tipo
         */
        void warnListeners(WindowType t);
        void addListener(Listener *listener, WindowType tipo);
        void wakeUp ( WindowType t, ... ) override;

        void creaShaderProgram(std::string &rutaShader);
        void creaModelo();

        int get_gladLoadGLLoader(void* procAddr);
        float get_gl_renderer();
        float get_gl_version();
        float get_gl_vendor();
        float get_gl_shading_language_version();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
