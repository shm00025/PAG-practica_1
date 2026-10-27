//
// Created by Santi on 05/10/2026.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <cstdarg>
#include <iostream>

#include "VentanaGestionModelosGUI.h"


namespace PAG {
    VentanaGestionModelosGUI::VentanaGestionModelosGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners), buscador(), modelos() {
        this->tipoVentana = WindowType::ModelGestor;
        this->buscador.SetTitle("Selecciona el modelo a usar");
        // Filtramos por obj dado que es la extensión que procesamos
        this->buscador.SetTypeFilters({".obj"});
    }

    void VentanaGestionModelosGUI::dibujarVentanaModelos() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(500, 200), ImGuiCond_Once);
        ImVec2 dummy_sz(20, 20); // Tamaño del espacio entre los cargadores y el lanzador

        if (ImGui::Begin("Gestion Modelos", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            // Recorremos la lista de modelos
            for (const std::string &modelo : this->modelos) {
                ImGui::Text(modelo.c_str());
                ImGui::SameLine();
                if (ImGui::Button("Borrar")) {
                    this->borrar = true;
                    this->nombreModelo = modelo;
                    // No hacemos el erase aquí dentro para evitar problemas de continuidad

                    // Limpiamos atributos para evitar problemas
                    this->rutaModelo = "";
                    this->seleccionShader = false;
                    this->seleccionShader = "";
                    this->seleccionModelo = false;

                    // Avisamos al renderer
                    warnListeners();
                }
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaGestionModelosGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_Once);
        ImVec2 dummy_sz(20, 20); // Tamaño del espacio entre los cargadores y el lanzador

        if (ImGui::Begin("Carga de Modelos", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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

            if (this->seleccionShader && this->seleccionModelo) {
                // Botón para realizar la carga del modelo con el shader
                if (ImGui::Button("Cargar Modelo y Shader")) {
                    this->borrar = false;
                    this->modelos.emplace(this->nombreModelo);
                    warnListeners(); // Avisamos a nuestros listeners (Renderer)

                    // Limpiamos la selección de modelo, la de shader no hace falta
                    this->nombreModelo = "";
                    this->rutaModelo = "";
                    this->seleccionModelo = false;
                }
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();

        // Segundo dibujar para la ventana de modelos
        if (!this->modelos.empty())
            dibujarVentanaModelos();
    }

    void VentanaGestionModelosGUI::warnListeners() {
        const char *cadenaShader = this->nombreShader.c_str();
        const char *cadenaModelo = this->rutaModelo.c_str();
        const char *cadenaNombre = this->nombreModelo.c_str();

        if (this->borrar) {
            // Borramos el modelo y avisamos
            this->modelos.erase(this->nombreModelo);
            for (Listener *listener: listeners) {
                listener->wakeUp(this->tipoVentana, 1, cadenaNombre);
            }
        } else {
            for (Listener *listener: listeners) {
                listener->wakeUp(this->tipoVentana, 0, cadenaShader, cadenaModelo, cadenaNombre);
            }
        }
    }

    void VentanaGestionModelosGUI::wakeUp(WindowType t, ...) {
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
