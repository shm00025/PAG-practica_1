//
// Created by Santi on 05/10/2026.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <cstdarg>

#include "VentanaSelectorShaderGUI.h"


namespace PAG {
    VentanaSelectorShaderGUI::VentanaSelectorShaderGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners) {
        this->tipoVentana = WindowType::ShaderSelector;
    };

    void VentanaSelectorShaderGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(400, 10), ImGuiCond_Once);

        if (ImGui::Begin("ShaderSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            // Dejamos la línea de input
            ImGui::InputText("##", &this->nombre, ImGuiInputTextFlags_AutoSelectAll);

            // Botón para cargar
            if (ImGui::Button("Load")) warnListeners(); // Avisamos a nuestros listeners (Renderer)
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaSelectorShaderGUI::warnListeners() {
        const char *cadena = this->nombre.c_str();
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, cadena);
        }
    }

    void VentanaSelectorShaderGUI::wakeUp(WindowType t, ...) {
        switch (t) {
            case WindowType::TextSize: {
                std::va_list args;
                va_start(args, t);

                const float* tam = va_arg(args, const float *);
                if (tam) this->tamTexto = *tam;

                va_end(args);
                break;
            }
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
}
