//
// Created by Santi on 07/10/2026.
//

#include <glad/glad.h>
#include <GL/gl.h>
#include <fstream>
#include <sstream>
#include <cstdarg>

#include "ShaderProgram.h"

#include <iostream>

std::string cargarShader(TipoShader tipo, std::string &ruta) {
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
            return "";
    }

    // Abrimos y leemos el fichero, si hay un error, lanzamos excepción
    std::ifstream archivoShader;
    archivoShader.open(ruta + terminacion);
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
    ShaderProgram::~ShaderProgram() {
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
        if (idVBOColor != 0) {
            glDeleteBuffers(1, &idVBOColor);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }

    void ShaderProgram::refrescar() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(idSP);
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    /**
    * Método para crear, compilar y enlazar el shader program
    * @note No se incluye ninguna comprobación de errores
    */
    void ShaderProgram::creaShaderProgram(std::string &rutaShader) {
        std::string codigoFuenteShader;

        try {
            // Creamos el vertex shader
            idVS = glCreateShader(GL_VERTEX_SHADER);
            if (idVS == 0 ) throw std::runtime_error("[error]: Error al crear el vertex shader, identificador nulo.");
            codigoFuenteShader = cargarShader(VertexShader, rutaShader);
            const GLchar *fuenteVS = codigoFuenteShader.c_str();
            glShaderSource(idVS, 1, &fuenteVS, nullptr);
            glCompileShader(idVS);
            consultarCompilacion(idVS, true);

            idFS = glCreateShader(GL_FRAGMENT_SHADER);
            if (idFS == 0 ) throw std::runtime_error("[error]: Error al crear el fragment shader, identificador nulo.");
            codigoFuenteShader = cargarShader(FragmentShader, rutaShader);
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

    void ShaderProgram::modeloVBONoEntrelazado(const GLfloat *vertices, const GLfloat *colores, int tamVector, int paso) {
        // Generamos el primer VBO, el de los vértices
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, paso, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), nullptr);

        // Generamos el segundo VBO, el de los colores
        glGenBuffers(1, &idVBOColor);
        glBindBuffer(GL_ARRAY_BUFFER, idVBOColor);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), colores, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, paso, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), nullptr);
    }

    void ShaderProgram::modeloVBOEntrelazado(const GLfloat *verticesConColor, int tamVector, int paso) {
        int size = paso / 2; // Dividmos el paso, que es el numero de elementos por vértice entre 2, que corresponden a cada atributo

        // Generamos el VBO para los datos entrelazados
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), verticesConColor, GL_STATIC_DRAW);

        // Activamos el atributo de los vértices e indicamos que es el primero y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), nullptr);

        // Activamos el atributo de los colores e indicamos que es el segundo, que empieza en la tercera posición y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), (void*)(size * sizeof(GLfloat)));
    }

    /**
    * Método para crear el VAO para el modelo a renderizar
    * @details -> Para VBO entrelazado la forma es Tipo, Indices, numDatos, numDatosPorVertice, VectorDatos
    * @details -> Para VBO NO entrelazado la forma es Tipo, Indices, numDatos, numDatosPorVertice, Vértices, Colores
    */
    void ShaderProgram::creaModelo(TipoVBO tipo_vbo, ...) {
        const GLuint *indices;
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        switch (tipo_vbo) {
            case Entrelazado: {
                std::va_list args;
                va_start(args, tipo_vbo);

                // Obtenemos los tres vectores necesarios
                indices = va_arg(args, GLuint *);
                int numDatos = va_arg(args, int);
                int datosPorVertice = va_arg(args, int);
                const GLfloat *verticesConColor = va_arg(args, GLfloat *);

                va_end(args);

                // Llamamos al fragmento específico de vbos entrelazados
                this->modeloVBOEntrelazado(verticesConColor, numDatos, datosPorVertice);
                break;
            }
            case NoEntrelazado: {
                std::va_list args;
                va_start(args, tipo_vbo);

                // Obtenemos los tres vectores necesarios
                indices = va_arg(args, GLuint *);
                int numDatos = va_arg(args, int);
                int datosPorVertice = va_arg(args, int);
                const GLfloat *vertices = va_arg(args, GLfloat *);
                const GLfloat *colores = va_arg(args, GLfloat *);

                va_end(args);

                // Llamamos al fragmento específico de vbos no entrelazados
                this->modeloVBONoEntrelazado(vertices, colores, numDatos, datosPorVertice);
                break;
            }
        }

        // Creamos y activamos el IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }
}