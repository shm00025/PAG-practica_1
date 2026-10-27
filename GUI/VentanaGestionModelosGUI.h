//
// Created by Santi on 05/10/2026.
//

#ifndef PRACTICA_1_VENTANASELECTORSHADERGUI_H
#define PRACTICA_1_VENTANASELECTORSHADERGUI_H

#include <string>
#include <set>

#include "VentanaGUI.h"
#include "imfilebrowser.h"


namespace PAG {
    class VentanaGestionModelosGUI : public VentanaGUI, public Listener {
        std::string nombreShader;
        bool seleccionShader = false;
        std::string nombreModelo;
        std::string rutaModelo;
        bool seleccionModelo = false;
        ImGui::FileBrowser buscador;

        bool borrar = false;
        std::set<std::string> modelos; // Set de nombre de modelos

        int accion = Carga;
        int transformacion = Traslacion;
        char valorTX[32], valorTY[32], valorTZ[32];
        bool moviendoModelos = false;

        void dibujarVentanaModelos();
        void dibujarVentanaTransformacion();

    public:
        VentanaGestionModelosGUI(const std::vector<Listener*> &listeners);
        ~VentanaGestionModelosGUI() override = default;

        void dibujar() override;

        bool getFlechasLibres() { return !this->moviendoModelos; }

        void warnListeners() override;
        void wakeUp(WindowType t, ...) override;
    };
}


#endif //PRACTICA_1_VENTANASELECTORSHADERGUI_H
