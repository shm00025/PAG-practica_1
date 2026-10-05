//
// Created by Santi on 27/09/2026.
//

#ifndef PRACTICA_1_LISTENER_H
#define PRACTICA_1_LISTENER_H

// Indicamos los tipos de ventana que tenemos actualmente en el sistema
enum WindowType {Renderer, General, Background, Console, TextSize, ShaderSelector};

namespace PAG {
    class Listener
    { public:
        Listener () = default;
        virtual ~Listener () = default;
        // WindowType es un tipo enumerado propio para identificar
        // el tipo de ventana de la interfaz
        virtual void wakeUp ( WindowType t, ... ) = 0;
    };
}


#endif //PRACTICA_1_LISTENER_H
