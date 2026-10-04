//
// Created by Santi on 04/10/2026.
//

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <cstdarg>

#include "VentanaFondoGUI.h"

#include <iostream>
#include <ostream>

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

            ImVec4 color = ImVec4(this->fondo[r], this->fondo[g], this->fondo[b], 1.0f);

            ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar;
            flags |= ImGuiColorEditFlags_PickerHueWheel;
            flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
            flags |= ImGuiColorEditFlags_NoAlpha;

            if (ImGui::ColorPicker4("MyColor##4", (float *) &color, flags)) {
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

    void VentanaFondoGUI::wakeUp(WindowType t, ...) {
        std::cout << "HOLA SOY EL FONDO"  << std::endl;
        switch (t) {
            case WindowType::Renderer: {
                std::va_list args;
                va_start(args, t);

                const std::vector<float> colorFondo = *(va_arg(args, std::vector<float> *));
                this->fondo = colorFondo;

                va_end(args);
                break;
            }
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
}
