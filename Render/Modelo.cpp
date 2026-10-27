//
// Created by Santi on 14/10/2026.
//

#include <cstdarg>
#include <iostream>
#include <ctime>
#include <sstream>

#include "Modelo.h"

#include <glm/ext/matrix_transform.hpp>


void traducirMatrizAssimpGLM(aiMatrix4x4 mAssimp, glm::mat4 &mat) {
    // Primero pasamos a glm la matriz de assimp
    glm::mat4 m = glm::mat4(0.0f);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            m[i][j] = mAssimp[i][j];
        }
    }

    // Comprobamos si es entera 0
    if (m == glm::mat4(0.0f)) {
        mat = glm::mat4(1.0f); // Si la matriz es cero la cambiamos por la identidad
        return;
    }
    mat = m; // Si no lo es copiamos los datos
}

namespace PAG {
    Modelo::~Modelo() {
        // Recorremos el vector de VBOs de manera inversa por si puediera dar errores
        for (int i = ((int) this->idVBOs.size() - 1); i >= 0; i--) {
            if (this->idVBOs[i] != 0) {
                glDeleteBuffers(1, &this->idVBOs[i]);
            }
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

        // Borramos los vectores dinámicos
        delete this->indices;
        delete this->vertices;
        delete this->colores;
        delete this->atributos;
    }

    void Modelo::procesarMalla(aiMesh *malla, std::vector<GLfloat> &atrib1, std::vector<GLfloat> &atrib2) {
        const int indicesAcumulados = (this->tipoVBO == Entrelazado ?
            (int) this->vectorAtributos.size() / 6 :
            (int) this->vectorVertices.size() / 3);

        for(unsigned int i = 0; i < malla->mNumFaces; i++) {
            // Para cada malla, estudiamos sus caras (Triángulos)
            const aiFace face = malla->mFaces[i];

            for(unsigned int j = 0; j < face.mNumIndices; j++)
                // Finalmente metemos los índices en el vector de índices
                // Usamos el offset para separar los ínices de cada malla porque van de [0, X]
                this->vectorIndices.push_back((GLint) face.mIndices[j] + indicesAcumulados);
        }

        for(unsigned int i = 0; i < malla->mNumVertices; i++) {
            // Para cada malla, estudiamos sus caras (Triángulos) y sus colores (Mediante las Normales)
            const aiVector3D vertice = malla->mVertices[i];
            const aiVector3D normal = malla->mNormals[i];

            // Copiamos los valores de las coordenadas
            atrib1.push_back(vertice.x);
            atrib1.push_back(vertice.y);
            atrib1.push_back(vertice.z);

            if (malla->mNormals) {
                // Copiamos los valores de las normales, si tiene, como colores (Crea un efecto chulo)
                atrib2.push_back(abs(normal.x));
                atrib2.push_back(abs(normal.y));
                atrib2.push_back(abs(normal.z));
            } else {
                // Si no tiene le damos un color base negro
                atrib2.push_back(255);
                atrib2.push_back(255);
                atrib2.push_back(255);
            }
        }
    }

    void Modelo::procesarNodo(aiNode *nodo, const aiScene *escena) {
        // Como nuestros modelos son simples, la matriz de modelado es común
        traducirMatrizAssimpGLM(nodo->mTransformation, this->mModelado); // Traducimos los tipos

        // Procesamos las mallas del nodo
        for(unsigned int i = 0; i < nodo->mNumMeshes; i++) {
            // Recorremos todas las mallas que contiene el modelo
            aiMesh *malla = escena->mMeshes[nodo->mMeshes[i]];
            procesarMalla(malla,
             this->tipoVBO == Entrelazado ? this->vectorAtributos : this->vectorVertices,
            this->tipoVBO == Entrelazado ? this->vectorAtributos : this->vectorColores);
        }

        // Llamada recursiva sobre sus hijso
        for(unsigned int i = 0; i < nodo->mNumChildren; i++) {
            procesarNodo(nodo->mChildren[i], escena);
        }
    }

    void Modelo::calcularVectores() {
        // Primero limpiamos los vectores por si contuviesen algo
        delete this->indices;
        delete this->vertices;
        delete this->colores;
        delete this->atributos;

        // Copiamos a un vector dinámico los índices
        this->indices = new GLuint[(int) this->vectorIndices.size()];
        for (int i = 0; i < this->vectorIndices.size(); ++i) this->indices[i] = this->vectorIndices[i];

        // Registramos el tamaño y limpiamos el vector para que no ocupe memoria
        this->tamVIndices = (int) this->vectorIndices.size();
        this->vectorIndices.clear();

        if (this->tipoVBO == Entrelazado) {
            this->atributos = new GLfloat[(int) this->vectorAtributos.size()];

            // Copiamos todos los datos entrelazados del vector std al dinámico
            for (int i = 0; i < this->vectorAtributos.size(); ++i) {
                this->atributos[i] = this->vectorAtributos[i];
            }

            // Registramos el tamaño y limpiamos el vector para que no ocupe memoria
            this->tamVEntrelazados = (int) this->vectorAtributos.size();
            this->vectorAtributos.clear();
        } else {
            this->vertices = new GLfloat[(int) this->vectorVertices.size()];
            this->colores = new GLfloat[(int) this->vectorColores.size()];

            // Copiamos todos los datos de los vectores std a los respectivos dinámicos
            for (int i = 0; i < this->vectorVertices.size(); ++i) {
                this->vertices[i] = this->vectorVertices[i];
                this->colores[i] = this->vectorColores[i];
            }

            // Registramos el tamaño y limpiamos el vector para que no ocupe memoria
            this->tamVNoEntrelazados = (int) this->vectorVertices.size();
            this->vectorVertices.clear();
            this->vectorColores.clear();
        }
    }

    std::string Modelo::asignarModelo(std::string &rutaOBJ, std::string &rutaShader, TipoVBO tipo) {
        this->tipoVBO = tipo; // Registramos el tipo de VBO a usar

        // Primero creamos el importador e importamos el fichero mediante su ruta
        Assimp::Importer importador;
        const aiScene *escena = importador.ReadFile(rutaOBJ,
                                                    aiProcess_JoinIdenticalVertices | aiProcess_Triangulate
                                                    | aiProcess_GenSmoothNormals);

        // Debemos comprobar si ha habido algún problema durante la carga del modelo
        if (!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
            throw std::runtime_error(importador.GetErrorString());

        // Inciamos el procesamiento recursivo de los nodos del modelo
        procesarNodo(escena->mRootNode, escena);

        // Finalmente pasamos los vectores de std a vectores dinámicos para su uso por parte de Opengl
        calcularVectores();

        // Creamos los respectivos VBOs
        int datosPorAtributo = 3;
        if (this->tipoVBO == Entrelazado)
            VBOEnt_CoordenadasYColor(this->indices, this->tamVEntrelazados, datosPorAtributo * 2, this->atributos);
        else
            VBONoEnt_CoordenadasYColor(this->indices, this->tamVNoEntrelazados, datosPorAtributo, this->vertices, this->colores);

        try {
            this->shaderModelo.creaShaderProgram(rutaShader);
        } catch (const std::exception &e) {
            std::string salida = e.what();
            salida.append("\n");
            throw std::runtime_error(salida);
        }

        // Finalmente como requisito de la práctica trasladamos unas coordenadas aleatorias el modelo
        std::srand(std::time(0)); // Creamos un aleatorio en función del tiempo

        // Aplicamos la transformación aleatoria
        glm::vec3 valores = glm::vec3(
            (factorEscaladoMA * (std::rand() % margenAleatorio) * (std::rand() % 2 == 0 ? (-1) : 1)),
            (factorEscaladoMA * (std::rand() % margenAleatorio) * (std::rand() % 2 == 0 ? (-1) : 1)),
            (factorEscaladoMA * (std::rand() % margenAleatorio) * (std::rand() % 2 == 0 ? (-1) : 1))
        );
        transformar(Traslacion, valores);

        // Indicamos la traslación
        std::stringstream ss;
        ss << "Modelo trasladado aleatoriamente. Valores: (" << valores.x << ", " << valores.y <<
            ", " << valores.z << ").\n";
        return ss.str();
    }

    void Modelo::transformar(int t, glm::vec3 valores) {
        // Hacemos los cambios pertinentes a la matriz de modelado
        switch (t) {
            case Traslacion: {
                // Aplicamos la traslación a la matriz de modelado
                this->mModelado = glm::translate(this->mModelado, valores);

                break;
            }
            case Escalado: {
                // Aplicamos el escalado a la matriz de modelado
                this->mModelado = glm::scale(this->mModelado, valores);

                break;
            }
            case Rotacion: {
                // Aplicamos cada rotación por separado a la matriz de modelado
                this->mModelado = glm::rotate(this->mModelado, glm::radians(valores.x), glm::vec3(1.0f, 0.0f, 0.0f));
                this->mModelado = glm::rotate(this->mModelado, glm::radians(valores.y), glm::vec3(0.0f, 1.0f, 0.0f));
                this->mModelado = glm::rotate(this->mModelado, glm::radians(valores.z), glm::vec3(0.0f, 0.0f, 1.0f));

                break;
            }
        }
        this->shaderModelo.actualizarMatrices(this->v * this->mModelado, this->pv * this->mModelado);
    }

    void Modelo::actualizarMatrices(glm::mat4 v, glm::mat4 pv) {
        this->v = v;
        this->pv = pv;
        this->shaderModelo.actualizarMatrices(v * this->mModelado, pv * this->mModelado);
    }

    void Modelo::refrescar() {
        this->shaderModelo.refrescar();

        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, this->tamVIndices, GL_UNSIGNED_INT, nullptr);
    }

    void Modelo::meterAtributoVBONoEntrelazado(const GLfloat *atributo, int i, int tamVector, int paso) {
        // Generamos el VBO número i
        glGenBuffers(1, &this->idVBOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, this->idVBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), atributo, GL_STATIC_DRAW);

        // Lo activamos y le damos las dimensiones de los datos
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, paso, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat), nullptr);
    }

    void Modelo::modeloVBOEntrelazado(const GLfloat *atributo, int tamVector) {
        // Generamos el VBO para los datos entrelazados
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, tamVector * sizeof(GLfloat), atributo, GL_STATIC_DRAW);
    }

    void Modelo::meterAtributoVBOEntrelazado(int i, int numAtributos, int paso) {
        int size = paso / numAtributos;

        // Activamos el atributo de los colores e indicamos que es el segundo, que empieza en la tercera posición y que tiene un paso de tamaño 6
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, paso * sizeof(GLfloat),
                              (void *) ((i * size) * sizeof(GLfloat)));
    }

    void Modelo::VBOEnt_CoordenadasYColor(GLuint *indices, int tamAtributoCompleto, int datosPorVertice,
                                          GLfloat *atributos) {
        int numAtributos = 2; // Porque solo usamos coordenadas y color

        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // Llamamos al fragmento específico de vbos entrelazados
        this->modeloVBOEntrelazado(atributos, tamAtributoCompleto); // Creamos el VBO con los datos
        this->meterAtributoVBOEntrelazado(0, numAtributos, datosPorVertice); // Indicamos cada atributo
        this->meterAtributoVBOEntrelazado(1, numAtributos, datosPorVertice); // Indicamos cada atributo

        // Creamos y activamos el IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->tamVIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }

    void Modelo::VBONoEnt_CoordenadasYColor(GLuint *indices, int tamAtributoCompleto, int datosPorVertice,
                                            GLfloat *vertices, GLfloat *colores) {
        int numAtributos = 2; // Porque solo usamos coordenadas y color

        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // Creamos espacio para todos los atributos
        this->idVBOs = std::vector<GLuint>(numAtributos, 0);

        // Creamos una entrada para cada atributo
        this->meterAtributoVBONoEntrelazado(vertices, 0, tamAtributoCompleto, datosPorVertice);
        this->meterAtributoVBONoEntrelazado(colores, 1, tamAtributoCompleto, datosPorVertice);

        // Creamos y activamos el IBO
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->tamVIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }
}
