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
        ImGui::SetNextWindowPos(ImVec2(400, 10), ImGuiCond_Once);

        // (*) En esta parte actuamos como creadores de información mediante nuestro menú de cámara
        if (ImGui::Begin("Camara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            ImGui::Combo("Item Type", &this->tipoCamara, opciones_camara, IM_COUNTOF(opciones_camara), IM_COUNTOF(opciones_camara));

            // TODO CONTINUAR
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaCamaraGUI::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, &this->tipoCamara, &this->valorX, &this->valorY);
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

                // (*) En esta parte actuamos unicamente como transmisores de información
                valorX = *(va_arg(args, const float *)); // Movimiento en X del ratón
                valorY = *(va_arg(args, const float *)); // Movimiento en Y del ratón
                warnListeners(); // Avisamos a renderer

                va_end(args);
                break;
            }
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
}