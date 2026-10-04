//
// Created by Santi on 27/09/2026.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "GUI.h"

#include "VentanaConsolaGUI.h"
#include "VentanaFondoGUI.h"

namespace PAG {
    PAG::GUI *PAG::GUI::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    GUI::GUI() : tipoVentana(General), ventanas() {
    }

    /**
    * Destructor
    */
    GUI::~GUI() {
    }

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    PAG::GUI &PAG::GUI::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if (!instancia) { instancia = new GUI(); }

        return *instancia;
    }

    void GUI::poner_linea(std::stringstream &linea) {
        // Pasamos el string por referencia para que sea más eficiente
        warnListeners(linea.str().c_str());
    }

    void GUI::inicializar(void* ventana, Listener* renderer) {
        // Creamos el contexto de ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Configuración con OpenGL y GLFW
        ImGui_ImplGlfw_InitForOpenGL ( (GLFWwindow*) ventana, true );
        ImGui_ImplOpenGL3_Init ();

        // Creamos las ventanas y añadimos sus listeners
        std::vector<Listener*> lista;
        ventanas.push_back(std::make_unique<VentanaConsolaGUI>(lista));
        this->listeners.push_back(dynamic_cast<Listener*>(ventanas[0].get())); // Aprovechamos para meter la consola como nuestro listener
        lista.push_back(renderer);
        ventanas.push_back(std::make_unique<VentanaFondoGUI>(lista));
    }

    void GUI::refrescar() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Se dibujan los controles de Dear ImGui
        for (const auto& ventana : ventanas) {
            ventana->dibujar();
        }

        // Aquí va el dibujado de la escena con instrucciones OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    void GUI::destruir() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }

    void GUI::warnListeners(const char *cadena) {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, cadena);
        }
    }
}
