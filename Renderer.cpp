//
// Created by Santi on 21/09/2026.
//

#include <glad/glad.h>
#include <GL/gl.h>
#include <cstdarg>
#include <iostream>
#include <GLFW/glfw3.h>

#include "GUI/GUI.h"
#include "Renderer.h"

constexpr float variacionColor[4] = {0.12, 0.06, 0.03, 1.0};
constexpr float margenInferior = 0, margenSuperior = 1;

/** Esta función implementa un comportamiento en ondas de los colores, con distintas longitudes, de manera
 *  que se van combinando los tres canales en todas sus posibles combinaciones. Las variaciones son todas
 *  múltiplos del mismo elemento para que coincidan en sus picos cada ciertas repeticiones.
 */
void actualizarColor(float *color, FlagsOndas* flags_propios, bool sentido) {
    for (int canal = 0; canal < numCanales; canal++) {
        float modificadorVariacion = 0; // El modificador por defecto es 0, si las actualizaciones no son seguras, no se cambia

        // Incluimos el modificador de sentido, que funciona como un override. Si el sentido es negativo, se invierten los flags
        bool modificadorSentido = (sentido ? flags_propios->flags[canal] : !flags_propios->flags[canal]);

        // La variable es redundante, pero sirve para que el código sea más claro
        if (modificadorSentido) {
            // Primero comprobamos si el color está por debajo del margen superior, si no lo está, se corta
            if ((color[canal] > margenSuperior) || ((color[canal] + variacionColor[canal]) > margenSuperior)) {
                color[canal] = margenSuperior;
                flags_propios->flags[canal] = !flags_propios->flags[canal]; // Invertimos el flag, hemos llegado a un límite
            } else {
                // Si la actualización es segura, se lleva a cabo
                modificadorVariacion = 1;
            }
        } else {
            // Primero comprobamos si el color está por encima del margen inferior, si no lo está, se corta
            if ((color[canal] < margenInferior) || ((color[canal] - variacionColor[canal]) < margenInferior)) {
                color[canal] = margenInferior;
                flags_propios->flags[canal] = !flags_propios->flags[canal]; // Invertimos el flag, hemos llegado a un límite
            } else {
                // Si la actualización es segura, se lleva a cabo
                modificadorVariacion = -1;
            }
        }

        // Las cláusulas de seguridad controlan el modificador, así que sabemos que la actualización es segura
        // Extraemos el código común a las cláusulas, si en un futuro es quiere cambiar la operación, solo se toca aquí
        color[canal] += (variacionColor[canal] * modificadorVariacion);
    }
}

namespace PAG {
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer() : tipoVentana(WindowType::Renderer) {
    }

    /**
    * Destructor
    */
    Renderer::~Renderer() {
    }

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    PAG::Renderer &PAG::Renderer::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if (!instancia) { instancia = new Renderer(); }

        return *instancia;
    }

    /**
    * Método para incializar opengl
    */
    void Renderer::inicializar() {
        // - Establecemos un gris medio como color con el que se borrará el
        // frame buffer.
        // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
        glClearColor(0.6, 0.6, 0.6, 1.0);
        // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de
        // dibujar.
        // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
        glEnable(GL_DEPTH_TEST);
        // - Ciclo de eventos de la aplicación. La condición de parada es que la
        // ventana principal deba cerrarse, por ejemplo, si el usuario pulsa el
        // botón de cerrar la ventana (la X).
    }

    /**
    * Método para hacer el refresco de la escena
    */
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Renderer::cerrar_ventana(void *ventana, int valor) {
        glfwSetWindowShouldClose((GLFWwindow *) ventana, valor);
    }

    void Renderer::scroll(void* flagsOndas, double xoffset, double yoffset) {
        // Primero. Debemos obtener el color actual de la ventana para modificarlo
        float color[4]; // Creamos un vector estático de flotantes para almacenar el color
        glGetFloatv(GL_COLOR_CLEAR_VALUE, color); // Consultamos el color a GL

        // Segundo. Obtenemos nuestros flags. Debemos hacer un cast a nuestro tipo dado que
        // el user pointer es un puntero void
        FlagsOndas* flags_ondas = (FlagsOndas*) flagsOndas;

        std::cout << "Color actual: (" << color[r] << ", " << color[g] << ", " << color[b] << ")" << std::endl;
        std::cout << "Flags ondas: (r: " << flags_ondas->flags[r]
                              << ", g: " << flags_ondas->flags[g]
                              << ", b: " << flags_ondas->flags[b]
                  << ")" << std::endl;
        bool sentido = yoffset > 0;
        std::cout << "Override: " << (sentido ? "False" : "True") << std::endl;

        // Actualizamos el color con la función asociada, de manera que modularizamos el código
        actualizarColor(color, flags_ondas, sentido);

        // Esta función ya aparece antes de lanzar la ventana para establecer el color base,
        // pero, aquí volvemos a llamarla cada vez que se detecta ele scroll para actualizar
        // el color de la ventana.
        glClearColor(color[r], color[g], color[b], 1.0);

        // Actualizamos el color de fondo
        this->colorFondo[r] = color[r];
        this->colorFondo[g] = color[g];
        this->colorFondo[b] = color[b];

        // Avisamos a nuestros listeners
        warnListeners();
    }

    void Renderer::addListener(Listener *listener) {
        listeners.push_back (listener);
    };

    void Renderer::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, &colorFondo);
        }
    }

    void Renderer::wakeUp(WindowType t, ...) {
        switch (t) {
            case WindowType::Background: {
                std::va_list args;
                va_start(args, t);
                colorFondo = *(va_arg(args, std::vector<float> *));
                // Finalmente pintamos el fondo
                glClearColor(colorFondo[r], colorFondo[g], colorFondo[b], 1.0);
                va_end(args);
                break;
            }
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }


    int Renderer::get_gladLoadGLLoader(void *procAddr) {
        return gladLoadGLLoader((GLADloadproc) procAddr);
    }

    float Renderer::get_gl_renderer() { return GL_RENDERER; }
    float Renderer::get_gl_version() { return GL_VERSION; }
    float Renderer::get_gl_vendor() { return GL_VENDOR; }
    float Renderer::get_gl_shading_language_version() { return GL_SHADING_LANGUAGE_VERSION; }
} // PAG
