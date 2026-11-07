//
// Created by Santi on 28/10/2026.
//

#ifndef PRACTICA_1_VENTANAMATERIALGUI_H
#define PRACTICA_1_VENTANAMATERIALGUI_H

#include <map>

#include "VentanaGUI.h"


namespace PAG {
    class VentanaMaterialGUI : public VentanaGUI, public Listener {
        std::map<std::string, Material> materiales;
        std::vector<char*> listaMat;

        ImVec4 cAmb = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        ImVec4 cDif = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        ImVec4 cEsp = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        char eEsp[32];

        ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

        int matSel = 0;
        std::string datoSeleccionado = "";
        std::string nombre_mat = "";

    public:
        VentanaMaterialGUI(const std::vector<Listener*> &listeners);
        ~VentanaMaterialGUI() override;

        void dibujar() override;

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANAMATERIALGUI_H
