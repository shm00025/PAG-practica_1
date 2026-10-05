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

#include <fstream>

constexpr float variacionColor[4] = {0.12, 0.06, 0.03, 1.0};
constexpr float margenInferior = 0, margenSuperior = 1;

const std::string rutaFuenteGLSL = "../shaders/pag03-";
const std::string sufijoVS = "vs.glsl";
const std::string sufijoFS = "fs.glsl";

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

std::string cargarShader(TipoShader tipo) {
    std::string terminacion, error;

    // Controlamos el tipo de shader a cargar, que influye en el fichero a leer y el error a msotrar
    switch (tipo) {
        case VertexShader:
            terminacion = sufijoVS;
            error = "[error]: Error al crear el vertex shader, no se puede abrir el archivo.";
            break;

        case FragmentShader:
            terminacion = sufijoFS;
            error = "[error]: Error al crear el fragment shader, no se puede abrir el archivo.";
            break;

        default:
            return nullptr;
    }

    // Abrimos y leemos el fichero, si hay un error, lanzamos excepción
    std::ifstream archivoShader;
    archivoShader.open(rutaFuenteGLSL + terminacion);
    if (!archivoShader.is_open()) throw std::runtime_error(error);

    std::stringstream streamShader;
    streamShader << archivoShader.rdbuf();

    // Cerramos el fichero y devolvemos el código fuente
    archivoShader.close();
    return streamShader.str();
}

void consultarCompilacion(GLint id, bool isShader) {
    GLint resultado = 0, tamMsj = 0;
    std::string mensaje;
    if (isShader) {
        glGetShaderiv(id, GL_COMPILE_STATUS, &resultado);
        mensaje = "[error]: Error indeterminado al compilar el shader.";
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &tamMsj);
    } else {
        glGetProgramiv (id, GL_LINK_STATUS, &resultado);
        mensaje = "[error]: Error indeterminado al enlazar los shaders.";
        glGetProgramiv ( id, GL_INFO_LOG_LENGTH, &tamMsj );
    }

    if (resultado == GL_FALSE) {
        /* Ha habido un error en la compilación.
          Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
          OpenGL */
        if (tamMsj > 0) {
            GLchar* mensajeFormatoC = new GLchar[tamMsj];
            GLint datosEscritos = 0;
            glGetShaderInfoLog(id, tamMsj, &datosEscritos
                                 , mensajeFormatoC);
            mensaje.assign(mensajeFormatoC);
            delete[] mensajeFormatoC;
            mensajeFormatoC = nullptr;
        }

        throw std::runtime_error(mensaje);
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
        if (idVS != 0) {
            glDeleteShader(idVS);
        }
        if (idFS != 0) {
            glDeleteShader(idFS);
        }
        if (idSP != 0) {
            glDeleteProgram(idSP);
        }
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
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
        glClearColor(this->colorFondo[r], this->colorFondo[g], this->colorFondo[b], this->colorFondo[alfa]);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
    }

    /**
    * Método para hacer el refresco de la escena
    */
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(idSP);
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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
        listeners.push_back(listener);
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

    /**
    * Método para crear, compilar y enlazar el shader program
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::creaShaderProgram() {
        std::string codigoFuenteShader;

        try {
            // Creamos el vertex shader
            idVS = glCreateShader(GL_VERTEX_SHADER);
            if (idVS == 0 ) throw std::runtime_error("[error]: Error al crear el vertex shader, identificador nulo.");
            codigoFuenteShader = cargarShader(VertexShader);
            const GLchar *fuenteVS = codigoFuenteShader.c_str();
            glShaderSource(idVS, 1, &fuenteVS, nullptr);
            glCompileShader(idVS);
            consultarCompilacion(idVS, true);

            idFS = glCreateShader(GL_FRAGMENT_SHADER);
            if (idFS == 0 ) throw std::runtime_error("[error]: Error al crear el fragment shader, identificador nulo.");
            codigoFuenteShader = cargarShader(FragmentShader);
            const GLchar *fuenteFS = codigoFuenteShader.c_str();
            glShaderSource(idFS, 1, &fuenteFS, nullptr);
            glCompileShader(idFS);
            consultarCompilacion(idFS, true);

            // Creamos el programa que contiene los shaders
            idSP = glCreateProgram();
            if (idSP == 0 ) throw std::runtime_error("[error]: Error al crear el shader program, identificador nulo.");
            glAttachShader(idSP, idVS);
            glAttachShader(idSP, idFS);
            glLinkProgram(idSP);
            consultarCompilacion(idSP, false);
        } catch (const std::exception& e) {
            std::string salida = e.what();
            salida.append("\n");
            throw std::runtime_error(salida);
        }
    }

    /**
    * Método para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::creaModelo() {
        GLfloat vertices[] = {
            -.5, -.5, 0,
            .5, -.5, 0,
            .0, .5, 0
        };
        GLfloat colores[] = {
            1.0, 0.6, 0.8,
            0.2, 1.0, 0.2,
            0.0, 0.0, 0.0
        };
        GLuint indices[] = {0, 1, 2};

        // Creamos y activamos el VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // Creamos y activamos el VBO no entrelazado
        /*
        // Generamos el primer VBO, el de los vértices
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

        // Generamos el segundo VBO, el de los colores
        glGenBuffers(1, &idVBOColor);
        glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colores), colores, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        */

        // Creamos y activamos el VBO entrelazado
        GLfloat verticesConColor[] = {
            -.5, -.5, 0, 1.0, 0.6, 0.8,
            .5, -.5, 0, 0.2, 1.0, 0.2,
            .0, .5, 0, 0.0, 0.0, 0.0
        };
        // Generamos el VBO para los datos entrelazados
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesConColor), verticesConColor, GL_STATIC_DRAW);

        // Activamos el atributo de los vértices e indicamos que es el primero y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);

        // Activamos el atributo de los colores e indicamos que es el segundo, que empieza en la tercera posición y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

        // Creamos y activamos el IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }


    float Renderer::get_gl_renderer() { return GL_RENDERER; }
    float Renderer::get_gl_version() { return GL_VERSION; }
    float Renderer::get_gl_vendor() { return GL_VENDOR; }
    float Renderer::get_gl_shading_language_version() { return GL_SHADING_LANGUAGE_VERSION; }
} // PAG
