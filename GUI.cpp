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
    }

    void GUI::refrescar() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Se dibujan los controles de Dear ImGui
        pintar_ventana_color();

        // Aquí va el dibujado de la escena con instrucciones OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    void GUI::destruir() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }

    void GUI::pintar_ventana_color() {
        // Posición de la ventana
        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );

        if ( ImGui::Begin ( "Paleta" ) ) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // Pintamos los controles
            static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;
            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            static bool ref_color = false;
            static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
            static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

            ImGuiColorEditFlags flags = base_flags | color_picker_flags;
            flags |= ImGuiColorEditFlags_PickerHueWheel;
            flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
            flags |= ImGuiColorEditFlags_NoAlpha;
            ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

            // Finalmente pintamos el fondo
            glClearColor(color.x, color.y, color.z, 1.0);
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }
}