//
// Created by Santi on 21/09/2026.
//

#ifndef PRACTICA_1_RENDERER_H
#define PRACTICA_1_RENDERER_H

#include <vector>
#include <map>

#include "ShaderProgram.h"
#include "Camara.h"
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

        glm::mat4 modeloProcesando; // Cambiar por una EEDD cuando haya varios modelos

        ShaderProgram shaderProgram;
        std::string mensajeError = "";

        Camara camara;

    public:
        virtual ~Renderer ();
        static Renderer& getInstancia ();

        void inicializar(GLint w, GLint h);
        void refrescar();

        void redimensionar(int width, int height);
        void cerrar_ventana(void *ventana, int valor);
        void scroll(void* flagsOndas, double xoffset, double yoffset);

        void warnListeners(WindowType t);
        void addListener(Listener *listener, WindowType tipo);
        void wakeUp ( WindowType t, ... ) override;

        void creaShaderProgram(std::string &rutaShader);
        void creaModelo();

        // TODO funcion que recibe datos desde la ventana de la interfaz de cámara de imgui y avisa a la cámara y después envía las nuevas matrices al shader program
        void actualizarCamara();

        int get_gladLoadGLLoader(void* procAddr);
        float get_gl_renderer();
        float get_gl_version();
        float get_gl_vendor();
        float get_gl_shading_language_version();
    };
} // PAG

#endif //PRACTICA_1_RENDERER_H
