//
// Created by Santi on 12/10/2026.
//

#include <imgui.h>
#include <cstdarg>
#include <iostream>

#include "VentanaCamaraGUI.h"
#include "../listener.h"

namespace PAG {
    VentanaCamaraGUI::VentanaCamaraGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners) {
        this->tipoVentana = WindowType::Camera;
    };

    void VentanaCamaraGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(100, 10), ImGuiCond_Once);

        if (ImGui::Begin("Camara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            const char* opciones_camara[] = {
                "Pan", "Tilt", "Dolly", "Crane", "Orbit", "Zoom"
            };
            static int item_type = 5; // Ponemos el Zoom por defecto
            ImGui::Combo("Item Type", &item_type, opciones_camara, IM_COUNTOF(opciones_camara), IM_COUNTOF(opciones_camara));

            // TODO CONTINUAR
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaCamaraGUI::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana);
        }
    }

    void VentanaCamaraGUI::wakeUp(WindowType t, ...) {
        switch (t) {
            case WindowType::TextSize: {
                std::va_list args;
                va_start(args, t);

                const float* tam = va_arg(args, const float *);
                if (tam) this->tamTexto = *tam;

                va_end(args);
                break;
            }
            case WindowType::General: {
                std::va_list args;
                va_start(args, t);

                const float movX = *(va_arg(args, const float *)); // Movimiento en X del ratón
                const float movY = *(va_arg(args, const float *)); // Movimiento en Y del ratón

                va_end(args);
                break;
            }
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
}