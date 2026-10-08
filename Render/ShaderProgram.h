//
// Created by Santi on 07/10/2026.
//

#ifndef PRACTICA_1_SHADERPROGRAM_H
#define PRACTICA_1_SHADERPROGRAM_H

#include<string>

enum TipoShader {VertexShader, FragmentShader};
enum TipoVBO {NoEntrelazado, Entrelazado};

const std::string rutaFuenteGLSL = "../shaders/pag03";
const std::string sufijoVS = "-vs.glsl";
const std::string sufijoFS = "-fs.glsl";

namespace PAG {
    class ShaderProgram {
        std::string nombreShader;

        // Render
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idVBOColor = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

        void modeloVBOEntrelazado(const GLfloat *verticesConColor, int numElementos, int paso);
        void modeloVBONoEntrelazado(const GLfloat *vertices, const GLfloat *colores, int numElementos, int paso);

    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        void refrescar();

        void creaShaderProgram(std::string &rutaShader);
        void creaModelo(TipoVBO tipo_vbo, ...);
    };
}


#endif //PRACTICA_1_SHADERPROGRAM_H
