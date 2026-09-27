//
// Created by Santi on 21/09/2026.
//

#include <glad/glad.h>
#include <GL/gl.h>
#include <cstdarg>
#include <iostream>
#include <GLFW/glfw3.h>

#include "GUI.h"
#include "Renderer.h"

namespace PAG {
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer() {
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

    void Renderer::scroll() {
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
