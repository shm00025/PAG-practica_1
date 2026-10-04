//
// Created by Santi on 04/10/2026.
//

#include <imgui.h>
#include <GLFW/glfw3.h>

#include "VentanaFondoGUI.h"
#include "../listener.h"

namespace PAG {
    VentanaFondoGUI::VentanaFondoGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners) {
        this->tipoVentana = WindowType::Background;
    };

    void VentanaFondoGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(400, 10), ImGuiCond_Once);

        if (ImGui::Begin("Paleta")) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(1.0f); // Escalamos el texto si fuera necesario

            // Pintamos los controles
            static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;
            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            static bool ref_color = false;
            static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
            static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

            ImGuiColorEditFlags flags = base_flags | color_picker_flags;
            flags |= ImGuiColorEditFlags_PickerHueWheel;
            flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
            flags |= ImGuiColorEditFlags_NoAlpha;
            if (ImGui::ColorPicker4("MyColor##4", (float *) &color, flags, ref_color ? &ref_color_v.x : NULL)) {
                this->fondo[r] = color.x;
                // Si el color de fondo ha cambiado guardamos el color y avisamos a los listeners
                this->fondo[r] = color.x;
                this->fondo[g] = color.y;
                this->fondo[b] = color.z;
                this->fondo[alfa] = color.w;

                this->warnListeners();
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaFondoGUI::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, &fondo);
        }
    }
}
