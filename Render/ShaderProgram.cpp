//
// Created by Santi on 07/10/2026.
//

#include <glad/glad.h>
#include <GL/gl.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "ShaderProgram.h"

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
    }

    void ShaderProgram::refrescar() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(idSP);

        // Indicamos los uniform a usar en el renderizado del modelo
        std::string nombreUniform = "matrizMVP";
        GLint posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
        if ( posicion != -1 )
        {  // El uniform existe y se ha podido localizar correctamente
            glUniformMatrix4fv ( posicion, 1, GL_FALSE, &this->matPVM[0][0] );
        }

        nombreUniform = "matrizMV";
        posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
        if ( posicion != -1 )
        {  // El uniform existe y se ha podido localizar correctamente
            glUniformMatrix4fv ( posicion, 1, GL_FALSE, &this->matVM[0][0] );
        }
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
}