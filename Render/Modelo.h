//
// Created by Santi on 14/10/2026.
//

#ifndef PRACTICA_1_MODELO_H
#define PRACTICA_1_MODELO_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ShaderProgram.h"


namespace PAG {
    class Modelo {
        TipoVBO tipoVBO;
        GLuint *indices = nullptr;
        std::vector<GLuint> vectorIndices;
        int tamVIndices = 0;

        GLfloat *vertices = nullptr;  // VBO no entrelazado
        std::vector<GLfloat> vectorVertices;
        GLfloat *colores = nullptr;   // VBO no entrelazado
        std::vector<GLfloat> vectorColores;
        int tamVNoEntrelazados = 0;

        GLfloat *atributos = nullptr; // VBO entrelazado
        std::vector<GLfloat> vectorAtributos;
        int tamVEntrelazados = 0;

        glm::mat4 v, pv; // Almacenamos las matrices de vision y proyeccion para actualizar las del shader en cambios de la de modelado
        glm::mat4 mModelado = glm::mat4(1.0f); // Por defecto es la identidad
        ShaderProgram shaderModelo;

        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object
        std::vector<GLuint> idVBOs = {0, 0};

        void procesarMalla(aiMesh *malla, std::vector<GLfloat> &atrib1, std::vector<GLfloat> &atrib2);
        void procesarNodo(aiNode *nodo, const aiScene *escena);
        void calcularVectores();

        void modeloVBOEntrelazado(const GLfloat *atributo, int tamVector);
        void meterAtributoVBOEntrelazado(int i, int numAtributos, int paso);
        void meterAtributoVBONoEntrelazado(const GLfloat *atributo, int i, int tamVector, int paso);

    public:
        Modelo() = default;
        ~Modelo();

        std::string asignarModelo(std::string &rutaOBJ, std::string &rutaShader, TipoVBO tipo);
        void actualizarMatrices(glm::mat4 v, glm::mat4 pv);

        void transformar(int t, glm::vec3 valores);
        void refrescar();

        void VBOEnt_CoordenadasYColor(GLuint *indices, int tamAtributoCompleto, int datosPorVertice, GLfloat *atributos);
        void VBONoEnt_CoordenadasYColor(GLuint *indices, int tamAtributoCompleto, int datosPorVertice, GLfloat *vertices, GLfloat *colores);
    };
}


#endif //PRACTICA_1_MODELO_H
