//
// Created by Santi on 21/09/2026.
//

#include <GL/gl.h>
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
} // PAG