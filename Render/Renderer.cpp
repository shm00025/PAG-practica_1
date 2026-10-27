//
// Created by Santi on 21/09/2026.
//

#include <glad/glad.h>
#include <GL/gl.h>
#include <cstdarg>
#include <sstream>
#include <GLFW/glfw3.h>
#include <fstream>

#include "Renderer.h"




/** Esta función implementa un comportamiento en ondas de los colores, con distintas longitudes, de manera
 *  que se van combinando los tres canales en todas sus posibles combinaciones. Las variaciones son todas
 *  múltiplos del mismo elemento para que coincidan en sus picos cada ciertas repeticiones.
 */
void actualizarColor(float *color, FlagsOndas *flags_propios, bool sentido) {
    for (int canal = 0; canal < numCanales; canal++) {
        float modificadorVariacion = 0;
        // El modificador por defecto es 0, si las actualizaciones no son seguras, no se cambia

        // Incluimos el modificador de sentido, que funciona como un override. Si el sentido es negativo, se invierten los flags
        bool modificadorSentido = (sentido ? flags_propios->flags[canal] : !flags_propios->flags[canal]);

        // La variable es redundante, pero sirve para que el código sea más claro
        if (modificadorSentido) {
            // Primero comprobamos si el color está por debajo del margen superior, si no lo está, se corta
            if ((color[canal] > margenSuperior) || ((color[canal] + variacionColor[canal]) > margenSuperior)) {
                color[canal] = margenSuperior;
                flags_propios->flags[canal] = !flags_propios->flags[canal];
                // Invertimos el flag, hemos llegado a un límite
            } else {
                // Si la actualización es segura, se lleva a cabo
                modificadorVariacion = 1;
            }
        } else {
            // Primero comprobamos si el color está por encima del margen inferior, si no lo está, se corta
            if ((color[canal] < margenInferior) || ((color[canal] - variacionColor[canal]) < margenInferior)) {
                color[canal] = margenInferior;
                flags_propios->flags[canal] = !flags_propios->flags[canal];
                // Invertimos el flag, hemos llegado a un límite
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
    Renderer::Renderer() : tipoVentana(WindowType::Renderer), listeners(), camara(), modelos() {
        for (const WindowType t : vectorWT) {
            this->listeners[t] = {}; // Creamos una lista vacía por cada tipo de listener
        }
    }

    /**
    * Destructor
    */
    Renderer::~Renderer() {}

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
    void Renderer::inicializar(GLint w, GLint h) {
        glClearColor(this->colorFondo[r], this->colorFondo[g], this->colorFondo[b], this->colorFondo[alfa]);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_MULTISAMPLE);

        this->camara.setWH(w, h);
    }

    /**
    * Método para hacer el refresco de la escena
    */
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (std::pair<const std::string, Modelo> &m : this->modelos)
            m.second.refrescar(); // Refrescamos cada modelo
    }

    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Renderer::cerrar_ventana(void *ventana, int valor) {
        glfwSetWindowShouldClose((GLFWwindow *) ventana, valor);
    }

    void Renderer::scroll(void *flagsOndas, double xoffset, double yoffset) {
        // Primero. Debemos obtener el color actual de la ventana para modificarlo
        float color[4]; // Creamos un vector estático de flotantes para almacenar el color
        glGetFloatv(GL_COLOR_CLEAR_VALUE, color); // Consultamos el color a GL

        // Segundo. Obtenemos nuestros flags. Debemos hacer un cast a nuestro tipo dado que
        // el user pointer es un puntero void
        FlagsOndas *flags_ondas = (FlagsOndas *) flagsOndas;

        std::stringstream ss;
        ss << "Color actual: (" << color[r] << ", " << color[g] << ", " << color[b] << ")" << std::endl;
        ss << "Flags ondas: (r: " << flags_ondas->flags[r]
                << ", g: " << flags_ondas->flags[g]
                << ", b: " << flags_ondas->flags[b]
                << ")" << std::endl;
        bool sentido = yoffset > 0;
        ss << "Override: " << (sentido ? "False" : "True") << std::endl;
        this->mensaje_consola = ss.str();
        warnListeners(WindowType::Console);

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
        warnListeners(WindowType::Background);
    }

    void Renderer::addListener(Listener *listener, WindowType tipo) {
        listeners[tipo].push_back(listener);
    };

    void Renderer::warnListeners(WindowType t) {
        switch (t) {
            case WindowType::Background: {
                for (Listener *listener: listeners[t]) {
                    listener->wakeUp(this->tipoVentana, &colorFondo);
                }
                break;
            }
            case WindowType::Console: {
                for (Listener *listener: listeners[t]) {
                    const char *cadena = this->mensaje_consola.c_str();
                    listener->wakeUp(this->tipoVentana, cadena);
                }
                break;
            }
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
            case WindowType::ModelCreator: {
                try {
                    std::va_list args;
                    va_start(args, t);

                    // Comprobamos si es un borrado o una carga //TODO =====================
                    int borrado = va_arg(args, int);

                    if (borrado == 1) {
                        // Seleccionamos el nombre
                        char *nombreModelo = va_arg(args, char *);
                        std::string nombreModeloS(nombreModelo);

                        // Borramos los datos
                        borraModelo(nombreModelo); // TODO IMPLEMENTAR
                        this->mensaje_consola = "\nBorrando el modelo " + nombreModeloS + ".\n\n";
                        warnListeners(WindowType::Console); // Indicamos la acción por la consola
                    } else {
                        // Seleccionamos el nombre
                        char *rutaShader = va_arg(args, char *);
                        char *rutaModelo = va_arg(args, char *);
                        char *nombreModelo = va_arg(args, char *);
                        std::string shaderString(rutaShader);
                        std::string modeloString(rutaModelo);
                        std::string nombreModeloS(nombreModelo);

                        // Creamos los datos
                        creaModelo(nombreModeloS, modeloString, shaderString);
                        this->mensaje_consola = "\nCargando el modelo " + nombreModeloS + " con el shader " + shaderString + ".\n\n";
                        warnListeners(WindowType::Console); // Indicamos la acción por la consola
                    }

                    va_end(args);
                } catch (const std::exception& e) {
                    this->mensaje_consola = e.what();
                    warnListeners(WindowType::Console);
                }
                break;
            }
            case WindowType::Camera: {
                std::va_list args;
                va_start(args, t);

                // Obtenemos los datos del menú de cámara o de los callbacks
                int tipoCamara = *(va_arg(args, int *));
                float valorX = *(va_arg(args, float *));
                float valorY = *(va_arg(args, float *));

                this->camara.movimiento(tipoCamara, valorX, valorY); // Indicamos el movimiento a la cámara
                glm::mat4 v = this->camara.getV();
                glm::mat4 pv = this->camara.getPV();
                for (std::pair<const std::string, Modelo> &m : this->modelos)
                    m.second.actualizarMatrices(v, pv); // Actualizamos cada modelo

                if (this->ultimaCamara != tipoCamara) {
                    this->ultimaCamara = tipoCamara; // Guardamos el cambio
                    // Indicamos el movimiento por consola
                    std::stringstream ss;
                    ss << "Cambio de cámara detectado. Cámara: " << opciones_camara[tipoCamara] << "." << std::endl;
                    this->mensaje_consola = ss.str();
                    warnListeners(WindowType::Console);
                }

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

    void Renderer::borraModelo(std::string nombreModelo) {
        this->modelos.erase(nombreModelo);
    }

    /**
    * Método para crear el modelo a renderizar
    */
    void Renderer::creaModelo(std::string nombreModelo, std::string &rutaOBJ, std::string &rutaShader) {
        try {
            // Insertamos el modelo en la estructura de datos
            Modelo &modelo = this->modelos[nombreModelo] = Modelo();

            // Elegimos entre VBO Entrelazado y no Entrelazado
            //modelo.asignarModelo(rutaOBJ, rutaShader, TipoVBO::Entrelazado);
            modelo.asignarModelo(rutaOBJ, rutaShader, TipoVBO::NoEntrelazado);

            // Indicamos el valor inicial de las matrices de la cámara
            modelo.actualizarMatrices(camara.getV(), camara.getPV());
        } catch (const std::exception& e) {
            std::string salida = e.what();
            salida.append("\n");
            throw std::runtime_error(salida);
        }
    }


    float Renderer::get_gl_renderer() { return GL_RENDERER; }
    float Renderer::get_gl_version() { return GL_VERSION; }
    float Renderer::get_gl_vendor() { return GL_VENDOR; }
    float Renderer::get_gl_shading_language_version() { return GL_SHADING_LANGUAGE_VERSION; }
} // PAG
