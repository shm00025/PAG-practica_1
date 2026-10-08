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
        // Recorremos el vector de VBOs de manera inversa por si puediera dar errores
        for (int i = (this->idVBOs.size() - 1); i >= 0; i--) {
            if (this->idVBOs[i] != 0) {
                glDeleteShader(this->idVBOs[i]);
            }
        }

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
/*
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
    }*/

    void ShaderProgram::meterAtributoVBONoEntrelazado(const GLfloat *atributo, int i, int tamVector, int paso) {
        // Generamos el VBO número i
        glGenBuffers(1, &this->idVBOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, this->idVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), atributo, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, paso, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), nullptr);
    }

    void ShaderProgram::modeloVBOEntrelazado(const GLfloat *atributo, int tamVector) {
        // Generamos el VBO para los datos entrelazados
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), atributo, GL_STATIC_DRAW);
    }

    void ShaderProgram::meterAtributoVBOEntrelazado(int i, int numAtributos, int paso) {
        int size = paso / numAtributos;

        // Activamos el atributo de los colores e indicamos que es el segundo, que empieza en la tercera posición y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), (void*)((i * size) * sizeof(GLfloat)));
    }

    /**
    * Método para crear el VAO para el modelo a renderizar
    * @details -> Para VBO entrelazado la forma es TipoShader, Indices, numAtributos, tamAtributoCompleto, datosPorVertice, vectorAtributos
    * @details -> Para VBO NO entrelazado la forma es TipoShader, Indices, numAtributos, tamAtributoCompleto, datosPorVertice, atributos...
    */
    void ShaderProgram::creaModelo(TipoVBO tipo_vbo, ...) {
        try {
            const GLuint *indices;
            glGenVertexArrays(1, &idVAO);
            glBindVertexArray(idVAO);

            switch (tipo_vbo) {
                case Entrelazado: {
                    std::va_list args;
                    va_start(args, tipo_vbo);

                    // Obtenemos los tres vectores necesarios
                    indices = va_arg(args, GLuint *);
                    int numAtributos = va_arg(args, int); // Se refiere a la lista (vertcies, colores, normales, ...)
                    int tamAtributoCompleto = va_arg(args, int); // Longitud de cada atributo (Ej: len(vertices) = len(clores) = ...)
                    int datosPorVertice = va_arg(args, int); // Datos que tiene cada vértice en el vector general
                    const GLfloat *atributo = va_arg(args, GLfloat *);

                    va_end(args);

                    // Llamamos al fragmento específico de vbos entrelazados
                    this->modeloVBOEntrelazado(atributo, tamAtributoCompleto); // Creamos el VBO con los datos
                    for (int i = 0; i < numAtributos; i++) {
                        this->meterAtributoVBOEntrelazado(i, numAtributos, datosPorVertice); // Indicamos cada atributo
                    }
                    break;
                }
                case NoEntrelazado: {
                    std::va_list args;
                    va_start(args, tipo_vbo);

                    // Obtenemos los tres vectores necesarios
                    indices = va_arg(args, GLuint *);
                    int numAtributos = va_arg(args, int); // Se refiere a la lista (vertcies, colores, normales, ...)
                    int tamAtributoCompleto = va_arg(args, int); // Longitud de cada atributo (Ej: len(vertices) = len(clores) = ...)
                    int datosPorVertice = va_arg(args, int); // Datos que tiene cada vértice en cada atributo

                    // Creamos espacio para todos los atributos
                    this->idVBOs = std::vector<GLuint>(numAtributos, 0);

                    // Creamos una entrada para cada atributo
                    int numeroAtributo = 0;
                    for (int i = 0; i < numAtributos; i++) {
                        const GLfloat *atributo = va_arg(args, GLfloat *);
                        if (atributo) {
                            std::cout << "Leyendo el dato: " << i << std::endl;
                            this->meterAtributoVBONoEntrelazado(atributo, numeroAtributo, tamAtributoCompleto, datosPorVertice);
                            numeroAtributo++;
                        } else {
                            throw std::runtime_error("[error]: Se ha definido un atributo nulo para el modelo.");
                        }
                    }

                    va_end(args);
                    break;
                }
            }

            // Creamos y activamos el IBO
            glGenBuffers(1, &idIBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

        } catch (const std::exception& e) {
            std::string salida = e.what();
            salida.append("\n");
            throw std::runtime_error(salida);
        }
    }
}