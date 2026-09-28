# PROGRAMACIÓN DE APLICACIONES GRÁFICAS
**Santiago Hervás del moral** `shm00025@red.ujaen.es`

---
## Práctica 1 - Configuración

---
### &rarr; Ejercicio de reflexión
El problema planteado se basa en diseñar una clase de C++ que encapsule los métodos relacionados al renderizado de la escena, con la dificultad de que los métodos de instancia de clase de C++ no se pueden usar como callbacks.

Los métodos asociados a las clases no se pueden usar como callbacks dado que dependen de una instancia de la clase. En C existen los métodos estáticos, que no dependen de la instancia, sino de la clase en sí, y no pueden contener llamadas a los métodos o utilizar las variables de instancia. Como las funciones estáticas son independientes, se pueden usar como callbacks.

El segundo problema surge de cómo desacoplar el uso de esta clase. La existencia de los callbacks está resuelta, pero si queremos asociarlos a la ventana deberemos realizar cada una de las llamadas y acceder a los métodos de la clase en un código que no debería conocerlos. Podemos solucionar este problema haciendo que sea la propia clase la que enlace los métodos con la venta, pasando en su constructor un puntero a ésta.

Finalmente, debería ser inicializado por el main cuando lance el sistema gráfico, dado que la clase contiene los callbacks de la ventana, y por lo tanto debe definirse al inicio.

A continuación se propone un diseño de esta clase:

![ver carpeta de diagramas](diagramas/DiagramaUML.png)
[Enlace al diagrama en la carpeta](diagramas/DiagramaUML.png)
---
## Práctica 3 - Primer Triángulo

---
### &rarr; Ejercicio de reflexión
El problema con la deformación del triángulo surge debido a la transofrmación de viewport. Tras el vertex shader, tenemos un espacio de recorte normalizado, y al aplicarle la transformación de viewport, estas coordenads normalizads se trasladan a coordenadas de pantalla. Si la pantalla cambia de coordenadas límite (Se redimensiona), el cálculo de las traslaciones también se ve afectado.

Como propuestas de solución he pensado en alterar el espacio de visión de la cámara, para que este se adapte al viewport que representa, de manera que si la ventana se alarga, el espacio de visión crezca de manera proprocional, y se mantenga la dimensión alterada.