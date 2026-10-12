//
// Created by Santi on 08/10/2026.
//

#ifndef PRACTICA_1_CONSTANTES_H
#define PRACTICA_1_CONSTANTES_H

#include <vector>
#include <string>

enum TipoShader {VertexShader, FragmentShader};
enum TipoVBO {NoEntrelazado, Entrelazado};

const std::string rutaFuenteGLSL = "../shaders/pag03";
const std::string sufijoVS = "-vs.glsl";
const std::string sufijoFS = "-fs.glsl";

constexpr int r = 0, g = 1, b = 2, alfa = 3;
constexpr int numCanales = 3;
constexpr float variacionColor[4] = {0.12, 0.06, 0.03, 1.0};
constexpr float margenInferior = 0, margenSuperior = 1;

// Indicamos los tipos de ventana que tenemos actualmente en el sistema
enum WindowType {Renderer, General, Background, Console, TextSize, ShaderSelector, Camera};
const std::vector<WindowType> vectorWT = {Renderer, General, Background, Console, TextSize, ShaderSelector, Camera};

inline const char* opciones_camara[] = { "Pan", "Tilt", "Dolly", "Crane", "Orbit", "Zoom" };
constexpr float factorConversionAngulo = 0.1;
constexpr float factorConversionDesplazamiento = 0.01;
constexpr float limiteTilt = 89.0f;
constexpr float minFovY = 0.1f, maxFovY = 90;

#endif //PRACTICA_1_CONSTANTES_H
