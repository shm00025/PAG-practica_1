//
// Created by Santi on 25/11/2026.
//

#ifndef PRACTICA_1_PROPIEDADESLUZ_H
#define PRACTICA_1_PROPIEDADESLUZ_H

namespace PAG {
    // Los datos devueltos son modificables
    class PropiedadesLuz {
        float Ia[3] = {1.0f, 1.0f, 1.0f};
        float Id[3] = {1.0f, 1.0f, 1.0f};
        float Is[3] = {1.0f, 1.0f, 1.0f};

        float p[3] = {0.0f, 0.0f, 0.0f};
        float d[3] = {0.0f, 0.0f, 0.0f};

        float gamma = 0.0f;
        float s = 0.0f;

    public:
        float *getIa() { return Ia; }
        float *getId() { return Id; }
        float *getIs() { return Is; }

        float *getP() { return p; }
        float *getD() { return d; }

        float &getGamma() { return gamma; }
        float &getS() { return s; }
    };
}


#endif //PRACTICA_1_PROPIEDADESLUZ_H
