#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    std::cout << "Starting Application PAG - Prueba 01" << std::endl;
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
    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);
    // - Ahora inicializamos GLAD.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow(window); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate();
        return -3;
    }
    // - Interrogamos a OpenGL para que nos informe de las propiedades del contexto
    // 3D construido.
    std::cout << glGetString(GL_RENDERER) << std::endl
            << glGetString(GL_VENDOR) << std::endl
            << glGetString(GL_VERSION) << std::endl
            << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    // - Establecemos un gris medio como color con el que se borrará el frame buffer.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glClearColor(0.6, 0.6, 0.6, 1.0);
    // - Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    // No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable(GL_DEPTH_TEST);
    // - Ciclo de eventos de la aplicación. La condición de parada es que la
    // ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
    // botón de cerrar la ventana (la X).
    while (!glfwWindowShouldClose(window)) {
        // - Borra los buffers (color y profundidad)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        // intercambia el buffer back (en el que se ha estado dibujando) por el
        // que se mostraba hasta ahora (front).
        glfwSwapBuffers(window);
        // - Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // - Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
    glfwDestroyWindow(window); // - Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
}