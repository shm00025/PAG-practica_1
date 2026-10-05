//
// Created by Santi on 04/10/2026.
//

#include <imgui.h>
#include <cstdarg>

#include "VentanaConsolaGUI.h"
#include "../listener.h"

namespace PAG {
    VentanaConsolaGUI::VentanaConsolaGUI(const std::vector<Listener *> &listeners) : VentanaGUI(listeners) {
        this->tipoVentana = WindowType::Console;
    };

    void VentanaConsolaGUI::ClearLog() {
        Items.str("");
        Items.clear();
    }

    void VentanaConsolaGUI::dibujar() {
        // Posición de la ventana
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);

        if (ImGui::Begin("Consola")) {
            ImGui::SetWindowFontScale(this->tamTexto); // Escalamos el texto si fuera necesario

            if (ImGui::SmallButton("Clear")) { ClearLog(); }
            ImGui::Separator();

            // Reserve enough left-over height for 1 separator + 1 input text
            ImGuiStyle &style = ImGui::GetStyle();
            const float footer_height_to_reserve = style.SeparatorSize + style.ItemSpacing.y +
                                                   ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened,
                                  ImGuiWindowFlags_HorizontalScrollbar)) {
                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::Selectable("Clear")) ClearLog();
                    ImGui::EndPopup();
                }

                // Display every line as a separate entry so we can change their color or add custom widgets.
                // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
                // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
                // to only process visible items. The clipper will automatically measure the height of your first item and then
                // "seek" to display only items in the visible area.
                // To use the clipper we can replace your standard loop:
                //      for (int i = 0; i < Items.Size; i++)
                //   With:
                //      ImGuiListClipper clipper;
                //      clipper.Begin(Items.Size);
                //      while (clipper.Step())
                //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                // - That your items are evenly spaced (same height)
                // - That you have cheap random access to your elements (you can access them given their index,
                //   without processing all the ones before)
                // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
                // We would need random-access on the post-filtered list.
                // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
                // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
                // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
                // to improve this example code!
                // If your items are of variable height:
                // - Split them into same height items would be simpler and facilitate random-seeking into your list.
                // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing

                std::istringstream stream(Items.str());
                std::string linea;

                while (std::getline(stream, linea)) {
                    ImVec4 color;
                    bool has_color = false;

                    if (linea.find("[error]") != std::string::npos) {
                        color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                        has_color = true;
                    } else if (linea.compare(0, 2, "#")) {
                        color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                        has_color = true;
                    }

                    if (has_color)
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(linea.c_str());
                    if (has_color)
                        ImGui::PopStyleColor();
                }

                // Siempre deslizamos si se puede
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);

                ImGui::PopStyleVar();
            }
            ImGui::EndChild();
            ImGui::Separator();
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }

    void VentanaConsolaGUI::warnListeners() {
        for (Listener *listener: listeners) {
            listener->wakeUp(this->tipoVentana);
        }
    }

    void VentanaConsolaGUI::wakeUp(WindowType t, ...) {
        switch (t) {
            case WindowType::General: {
                std::va_list args;
                va_start(args, t);

                const char* cadena = va_arg(args, const char *);
                if (cadena) {
                    // La añadimos al stream
                    this->Items << cadena;
                }

                va_end(args);
                break;
            }
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
