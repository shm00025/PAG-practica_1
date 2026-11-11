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
        char eEsp[32] = {};

        ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

        int matSel = 0;
        std::string datoSeleccionado = "";  // Material seleccionado para editar
        std::string nombre_mat = "";        // Nombre para un nuevo material

        bool nuevo = true;                  // Indicador de si es nuevo o edición (Depuración por consola)
        std::string nombre_envio = "";      // Nombre a enviar al gestor de modelos, sea nuevo o edición

    public:
        VentanaMaterialGUI(const std::vector<Listener*> &listeners);
        ~VentanaMaterialGUI() override;

        void dibujar() override;

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANAMATERIALGUI_H
