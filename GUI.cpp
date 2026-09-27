//
// Created by Santi on 27/09/2026.
//

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "GUI.h"

namespace PAG {
    PAG::GUI *PAG::GUI::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    GUI::GUI() {
    }

    /**
    * Destructor
    */
    GUI::~GUI() {
    }

    /**
    * Consulta del objeto único de la clase
    * @return La dirección de memoria del objeto
    */
    PAG::GUI &PAG::GUI::getInstancia() {
        // Lazy initialization: si aún no existe, lo crea
        if (!instancia) { instancia = new GUI(); }

        return *instancia;
    }

    void GUI::poner_linea(std::stringstream &linea) {
        // Pasamos el string por referencia para que sea más eficiente
        Items << linea.str();
    }

    void GUI::inicializar(void* ventana) {
        // Creamos el contexto de ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        // Configuración con OpenGL y GLFW
        ImGui_ImplGlfw_InitForOpenGL ( (GLFWwindow*) ventana, true );
        ImGui_ImplOpenGL3_Init ();
    }

    void GUI::refrescar() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Se dibujan los controles de Dear ImGui
        pintar_ventana_color();
        pintar_consola();

        // Aquí va el dibujado de la escena con instrucciones OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

    void GUI::destruir() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext ();
    }

    void GUI::pintar_ventana_color() {
        // Posición de la ventana
        ImGui::SetNextWindowPos ( ImVec2 (400, 10), ImGuiCond_Once );

        if ( ImGui::Begin ( "Paleta" ) ) {
            // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            // Pintamos los controles
            static ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;
            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            static bool ref_color = false;
            static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
            static ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_AlphaBar;

            ImGuiColorEditFlags flags = base_flags | color_picker_flags;
            flags |= ImGuiColorEditFlags_PickerHueWheel;
            flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
            flags |= ImGuiColorEditFlags_NoAlpha;
            if (ImGui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL)) {this->fondo[r] = color.x;
                // Si el color de fondo ha cambiado guardamos el color y avisamos a los listeners
                this->fondo[r] = color.x;
                this->fondo[g] = color.y;
                this->fondo[b] = color.z;
                this->fondo[alfa] = color.w;

                this->warnListeners();
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }

    void GUI::ClearLog() {
        Items.clear();
    }

    void GUI::pintar_consola() {
        // Posición de la ventana
        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );

        if ( ImGui::Begin ( "Consola" ) ) {
            if (ImGui::SmallButton("Clear"))           { ClearLog(); }
            ImGui::Separator();

            // Reserve enough left-over height for 1 separator + 1 input text
            ImGuiStyle& style = ImGui::GetStyle();
            const float footer_height_to_reserve = style.SeparatorSize + style.ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar))
            {
                if (ImGui::BeginPopupContextWindow())
                {
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

                    if (linea.find("[error]") != std::string::npos) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                    else if (linea.compare(0, 2, "#")) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }

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
        ImGui::End ();
    }

    void GUI::addListener(Listener *listener) {
        listeners.push_back (listener);
    }

    void GUI::warnListeners() {
        for (Listener* listener: listeners) {
            listener->wakeUp (WindowType::Background, &fondo);
        }
    }
}