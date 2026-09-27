//
// Created by Santi on 27/09/2026.
//

#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace PAG {
    PAG::GUI *PAG::GUI::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    GUI::GUI() {
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

    void GUI::inicializar(void* ventana) {
        // Creamos el contexto de ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Configuración con OpenGL y GLFW
        ImGui_ImplGlfw_InitForOpenGL ( (GLFWwindow*) ventana, true );
        ImGui_ImplOpenGL3_Init ();

        // Posición de la ventana
        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );
    }

    void GUI::refrescar() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Se dibujan los controles de Dear ImGui
        // Aquí va el dibujado de la escena con instrucciones OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    void GUI::destruir() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }
}