#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <GLFW/glfw3.h>

#include "GUI/GUI.h"
#include "Renderer.h"

// ----------- FUNCIONES DEL GUIÓN -----------
// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback(int errno, const char *desc) {
    std::string aux(desc);
    std::stringstream ss;
    ss << "Error de GLFW número " << errno << ": " << aux << std::endl;
    PAG::GUI::getInstancia().poner_linea(ss);
}

// - Esta función callback será llamada cada vez que se cambie el tamaño
// del área de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    PAG::Renderer::getInstancia().redimensionar(width, height);
    std::stringstream ss;
    ss << "Resize callback called" << std::endl;
    PAG::GUI::getInstancia().poner_linea(ss);
}

// - Esta función callback será llamada cada vez que se pulse una tecla
// dirigida al área de dibujo OpenGL.
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        PAG::Renderer::getInstancia().cerrar_ventana(window, GLFW_TRUE);
    }
    std::stringstream ss;
    ss << "Key callback called" << std::endl;
    PAG::GUI::getInstancia().poner_linea(ss);
}

// - Esta función callback será llamada cada vez que se pulse algún botón
// del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        std::stringstream ss;
        ss << "Pulsado el botón: " << button << std::endl;
        PAG::GUI::getInstancia().poner_linea(ss);
    } else if (action == GLFW_RELEASE) {
        std::stringstream ss;
        ss << "Soltado el botón: " << button << std::endl;
        PAG::GUI::getInstancia().poner_linea(ss);
    }
}

void callbackRefrescoVentana ( GLFWwindow* ventana ) {
    // Refrescamos nuestros dos sistemas
    PAG::Renderer::getInstancia().refrescar();
    PAG::GUI::getInstancia().refrescar();

    glfwSwapBuffers (ventana);
}

// Función callback para el scroll realizado con la rueda del ratón.
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    std::cout << "Movida la rueda del ratón " << xoffset
            << " Unidades en horizontal y " << yoffset
            << " unidades en vertical" << std::endl;
    PAG::Renderer::getInstancia().scroll(glfwGetWindowUserPointer(window), xoffset, yoffset);
}

// Función callback para el scroll realizado con la rueda del ratón.
void scroll_callback_simple(GLFWwindow *window, double xoffset, double yoffset) {
    std::stringstream ss;
    ss << "Movida la rueda del ratón " << xoffset
       << " Unidades en horizontal y " << yoffset
       << " unidades en vertical" << std::endl;
    PAG::GUI::getInstancia().poner_linea(ss);
}


int main() {
    std::cout << "Starting Application PAG - Prueba 01" << std::endl;
    // - Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback((GLFWerrorfun) error_callback);


    // - Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }


    // - Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el
    // modo Core Profile.
    glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialiasing con 4 muestras.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // - Esta y las 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL Core Profile 4.3.
    // - Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;
    // - Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // sin compartir recursos con otras ventanas.
    window = glfwCreateWindow(1024, 576, "PAG Introduction", nullptr, nullptr);
    // - Comprobamos si la creación de la ventana ha tenido éxito.
    if (window == nullptr) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // Aprovechamos para indicar nuestros propios flags
    FlagsOndas flags_ondas;
    glfwSetWindowUserPointer(window, &flags_ondas);


    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);
    // - Ahora inicializamos GLAD.
    if (!PAG::Renderer::getInstancia().get_gladLoadGLLoader((void*) glfwGetProcAddress)) {
        std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow(window); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate();
        return -3;
    }
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::cout << glGetString(PAG::Renderer::getInstancia().get_gl_renderer()) << std::endl
            << glGetString(PAG::Renderer::getInstancia().get_gl_vendor()) << std::endl
            << glGetString(PAG::Renderer::getInstancia().get_gl_version()) << std::endl
            << glGetString(PAG::Renderer::getInstancia().get_gl_shading_language_version()) << std::endl;
    std::cout << glGetString(PAG::Renderer::getInstancia().get_gl_shading_language_version()) << std::endl;
    // - Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback(window, callbackRefrescoVentana);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Inicializamos OpenGL
    PAG::Renderer::getInstancia().inicializar();
    PAG::Renderer::getInstancia().creaShaderProgram();
    PAG::Renderer::getInstancia().creaModelo();

    // Creamos la interfaz mediante IMGUI
    PAG::GUI::getInstancia().inicializar(window, &PAG::Renderer::getInstancia());

    while (!glfwWindowShouldClose(window)) {
        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones
        // de teclas o de ratón, etc. Siempre al final de cada iteración del
        // ciclo de eventos y después de glfwSwapBuffers ( window );
        glfwPollEvents();

        // Invocamos el callback para que se redibuje la interfaz y permitir las animaciones
        callbackRefrescoVentana(window);
    }
    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    PAG::GUI::getInstancia().destruir();
    glfwDestroyWindow(window); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
}