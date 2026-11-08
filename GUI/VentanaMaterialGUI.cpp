//
// Created by Santi on 28/10/2026.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <cstdarg>
#include <iostream>

#include "VentanaMaterialGUI.h"
#include "../listener.h"

void pasarImVec4aMaterial(Material &mat, ImVec4 &cAmb, ImVec4 &cDif, ImVec4 &cEsp, char *eEsp) {
    float espF = std::strtof(eEsp, nullptr); // Traducimos el tipo a float
    // Copiamos cada campo
    mat.cAmbiente[0] = cAmb.x; mat.cAmbiente[1] = cAmb.y; mat.cAmbiente[2] = cAmb.z;
    mat.cDifuso[0] = cDif.x; mat.cDifuso[1] = cDif.y; mat.cDifuso[2] = cDif.z;
    mat.cEspecular[0] = cEsp.x; mat.cEspecular[1] = cEsp.y; mat.cEspecular[2] = cEsp.z;
    mat.eEspecular = espF;
}

void pasarMaterialaImVec4(Material &mat, ImVec4 &cAmb, ImVec4 &cDif, ImVec4 &cEsp, char *eEsp) {
    // Copiamos cada campo
    cAmb.x = mat.cAmbiente[0]; cAmb.y = mat.cAmbiente[1]; cAmb.z = mat.cAmbiente[2];
    cDif.x = mat.cDifuso[0]; cDif.y = mat.cDifuso[1]; cDif.z = mat.cDifuso[2];
    cEsp.x = mat.cEspecular[0]; cEsp.y = mat.cEspecular[1]; cEsp.z = mat.cEspecular[2];
    snprintf(eEsp, sizeof(eEsp), "%f", mat.eEspecular); // Copiamos el dato al buffer
}

namespace PAG {
    VentanaMaterialGUI::VentanaMaterialGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners), materiales() {
        this->tipoVentana = WindowType::Materials;
    };

    VentanaMaterialGUI::~VentanaMaterialGUI() {
        for (char* opcion : this->listaMat) {
            free(opcion);
        }
    }

    void VentanaMaterialGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(700, 100), ImGuiCond_Once);

        if (ImGui::Begin("Material", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            if (ImGui::BeginTabBar("TabModosMaterial")) {
                if (ImGui::BeginTabItem("Crear Material")) {
                    // Cremaos el menú por coordenada
                    ImGui::ColorEdit3("Color Ambiente", (float*)&cAmb, base_flags);
                    ImGui::ColorEdit3("Color Difuso", (float*)&cDif, base_flags);
                    ImGui::ColorEdit3("Color Especular", (float*)&cEsp, base_flags);

                    // Exponente especular
                    ImGui::InputText("Exponente", eEsp, IM_COUNTOF(eEsp), ImGuiInputTextFlags_CharsDecimal);

                    // Nombre del material
                    ImGui::InputText("##", &this->nombre_mat, ImGuiInputTextFlags_AutoSelectAll);

                    // Si hemos asignado un nombre podemos guardar el material
                    if (!this->nombre_mat.empty())
                        if (ImGui::Button("Guardar")) {
                            Material nuevo;
                            pasarImVec4aMaterial(nuevo, this->cAmb, this->cDif, this->cEsp, this->eEsp);
                            this->materiales.insert({this->nombre_mat, nuevo});

                            // Añadimos el dato a la lista del selector
                            // Copiamos el str para evitar problemas de punteros
                            this->listaMat.push_back(strdup(this->nombre_mat.c_str()));
                            this->nombre_envio = nombre_mat;
                            this->nuevo = true;

                            warnListeners(); // Enviamos el dato incremental al gestor de modelos
                        }

                    ImGui::EndTabItem();
                }

                if (!this->listaMat.empty()) {
                    if (ImGui::BeginTabItem("Modificar Material")) {
                        std::string seleccion = this->listaMat[this->matSel];
                        char **datos = this->listaMat.data(); // Obtenemos un vector básico para la función de ImGUI
                        ImGui::Combo("Selección", &this->matSel, datos, this->listaMat.size(), this->listaMat.size());

                        if (this->datoSeleccionado != seleccion) { // Si hemos cambiado de selección o se ha refrescado
                            // Copiamos los datos del material almacenado a los medidores
                            pasarMaterialaImVec4(this->materiales.find(seleccion)->second,
                                this->cAmb, this->cDif, this->cEsp, this->eEsp);
                            this->datoSeleccionado = seleccion;
                        }

                        // Cremaos el menú por coordenada
                        ImGui::ColorEdit3("Color Ambiente", (float*)&cAmb, base_flags);
                        ImGui::ColorEdit3("Color Difuso", (float*)&cDif, base_flags);
                        ImGui::ColorEdit3("Color Especular", (float*)&cEsp, base_flags);

                        // Exponente especular
                        ImGui::InputText("Exponente", eEsp, IM_COUNTOF(eEsp), ImGuiInputTextFlags_CharsDecimal);

                        // Guardamos los cambios
                        if (ImGui::Button("Guardar")) {
                            Material nuevo;
                            pasarImVec4aMaterial(nuevo, this->cAmb, this->cDif, this->cEsp, this->eEsp);
                            this->materiales[seleccion] = nuevo; // Actualizamos el dato
                            this->nombre_envio = this->datoSeleccionado;
                            this->nuevo = false;

                            warnListeners(); // Enviamos el dato incremental al gestor de modelos
                        }

                        ImGui::EndTabItem();
                    }
                }

                ImGui::EndTabBar();
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaMaterialGUI::warnListeners() {
        // Usamos directamente el find dado que acabamos de meter el dato y sabemos que existe
        Material mat = this->materiales.find(this->nombre_envio)->second;

        for (Listener *listener: listeners) {
            // Hacemos envíos incrementales, solo el nuevo nombre y material
            // En el caso de una actualización funciona igual, dado que se gestionan igual en el destino
            listener->wakeUp(this->tipoVentana, this->nombre_envio.c_str(), mat.cAmbiente, mat.cDifuso,
                mat.cEspecular, mat.eEspecular, this->nuevo);
        }

        // Tras el envío limpiamos el nombre y la selección
        this->nombre_mat = "";
        this->datoSeleccionado = "";
        this->nombre_envio = "";
    }

    void VentanaMaterialGUI::wakeUp(WindowType t, ...) {
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