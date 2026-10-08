//
// Created by Santi on 05/10/2026.
//

#include <imgui.h>

#include "VentanaTamTexto.h"

namespace PAG {
    VentanaTamTexto::VentanaTamTexto(const std::vector<Listener *> &listeners) : VentanaGUI(listeners) {
        this->tipoVentana = WindowType::TextSize;
    };

    void VentanaTamTexto::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(400, 10), ImGuiCond_Once);

        if (ImGui::Begin("TamTexto")) {
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            ImGui::SliderFloat("Escala", &this->tamTexto, 0.01f, 5.0f, "%.3f");

            warnListeners();
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaTamTexto::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, &this->tamTexto);
        }
    }
}