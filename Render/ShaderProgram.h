//
// Created by Santi on 07/10/2026.
//

#ifndef PRACTICA_1_SHADERPROGRAM_H
#define PRACTICA_1_SHADERPROGRAM_H

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../constantes.h"

namespace PAG {
    class ShaderProgram {
        std::string nombreShader;

        // Render
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program

        glm::mat4 matVM, matPVM;

    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        void refrescar();
        void creaShaderProgram(std::string &rutaShader);

        void actualizarMatrices(glm::mat4 vm, glm::mat4 pvm) { this->matVM = vm, this->matPVM = pvm; }
    };
}


#endif //PRACTICA_1_SHADERPROGRAM_H
