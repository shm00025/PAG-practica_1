//
// Created by Santi on 05/10/2026.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <cstdarg>
#include <iostream>

#include "VentanaSelectorShaderGUI.h"


namespace PAG {
    VentanaSelectorShaderGUI::VentanaSelectorShaderGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners), buscador() {
        this->tipoVentana = WindowType::ModelCreator;
        this->buscador.SetTitle("Selecciona el modelo a usar");
        // Filtramos por obj dado que es la extensión que procesamos
        this->buscador.SetTypeFilters({".obj"});
    };

    void VentanaSelectorShaderGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_Once);
        ImVec2 dummy_sz(20, 20); // Tamaño del espacio entre los cargadores y el lanzador

        if (ImGui::Begin("ShaderSelector", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            // Dejamos la línea de input
            ImGui::InputText("##", &this->nombreShader, ImGuiInputTextFlags_AutoSelectAll);

            // Botón para cargar
            if (ImGui::Button("Load")) {
                this->seleccionShader = true;
            }
            ImGui::SameLine();
            // Botón para cargar preset
            if (ImGui::Button("Load-PAG03")) {
                this->nombreShader = rutaFuenteGLSL;
                this->seleccionShader = true;
            }

            // Dibujamos el buscador
            this->buscador.Display();
            // Botón para cargar el modelo
            if (ImGui::Button("Modelo")) this->buscador.Open();
            ImGui::SameLine();
            std::string cadena = (seleccionModelo ? this->nombreModelo : "Aún no hay selección");
            ImGui::Text(cadena.c_str());

            // Si hay selección la guardamos
            if (this->buscador.HasSelected()) {
                std::filesystem::path ruta = this->buscador.GetSelected();
                this->seleccionModelo = true;

                // Guardamos la selección
                this->nombreModelo = ruta.filename().string();
                this->rutaModelo = ruta.string();
                this->buscador.ClearSelected();
            }

            ImGui::Dummy(dummy_sz);

            if (this->seleccionShader && this->seleccionModelo)
                // Botón para realizar la carga del modelo con el shader
                if (ImGui::Button("Cargar Modelo y Shader"))
                    warnListeners(); // Avisamos a nuestros listeners (Renderer)
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaSelectorShaderGUI::warnListeners() {
        const char *cadenaShader = this->nombreShader.c_str();
        const char *cadenaModelo = this->rutaModelo.c_str();
        const char *cadenaNombre = this->nombreModelo.c_str();
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana, cadenaShader, cadenaModelo, cadenaNombre);
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
