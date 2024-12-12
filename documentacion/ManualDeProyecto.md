# Manual de Proyecto

## Introducción

Este documento tiene como objetivo presentar la división de tareas a lo largo del proyecto, así como la planificación de las mismas. Se presentarán las tareas a realizar, el tiempo estimado para cada una de ellas y el responsable de llevarlas a cabo. Asimismo, indicar las herramientas utilizadas para la realización de las tareas y la metodología de trabajo.

#### Santiago Domínguez

- Protocolos y sus respestivos tests.
- Manejo del YAML de los mapas, para que el servidor pueda utilizar esa información para incorporar correctamente las ubicaciones de las armas dispuestas, los respawn, etc.


#### Federico Nemirovsky
- Gameloop del server.
- Protocolo del lobby del lado del cliente.
- Planteo inicial de la implementación gráfica del lobby.


#### Franco Rodriguez



#### Franco Valfré



### Planeación del Trabajo Práctico

En las primeras dos semanas del proyecto, acordamos dividir las responsabilidades de la siguiente manera: dos integrantes se encargarían de la implementación del servidor, mientras que los otros dos se centrarían en la implementación del cliente. Sin embargo, conforme avanzamos en el desarrollo, decidimos adoptar un enfoque más colaborativo. Esto nos permitió intercambiar perspectivas y enriquecer el trabajo en ambas áreas, promoviendo una revisión cruzada entre todos los miembros del equipo.

Esta metodología continuó hasta que iniciamos el desarrollo del editor de mapas. En esta etapa, asignamos a un integrante la responsabilidad exclusiva de esta tarea, mientras que otro se encargó de garantizar que el servidor pudiera leer correctamente los archivos en formato YAML. El resto del equipo se dedicó a mejorar y optimizar las funcionalidades tanto del servidor como del cliente. Este enfoque permitió avanzar de manera más eficiente en las distintas partes del proyecto, asegurando la calidad y la integración de los componentes desarrollados.


### Principales Problemas

Hubo dos grandes problemas en el desarrollo del proyecto. El primero fue la inclusion de la libreria SDL2, que nos llevaría a modificar los archivos de compilacion de Makefile y CMake para que pudiera compilar correctamente en todos los ambientes. 

Por otro lado, el lobby grafico tambien causo problemas, ya que al no tener experiencia previa con esta herramienta (QT5) y el tiempo limitado, nos llevo mucho tiempo poder implementarlo correctamente. 



### Herramientas utilizadas

- Visual Studio Code: Editor de texto utilizado para programar, consume bajos recursos y es muy eficiente.
- CppCheck: Herramienta utilizada para chequear errores en el código.
- ClangFormat: Herramienta utilizada para formatear el código.
- CPPLint: Herramienta utilizada para tener codigo limpio y ordenado.
- Github Actions: Herramienta utilizada para chequear errores en el código y formatear el mismo.
- SDL2: Libreria utilizada para la creación de la interfaz gráfica.
- QT5: Libreria utilizada para la creación de la interfaz gráfica.
- Google Tests: Framework utilizado para realizar tests unitarios de los protocolos.

Además, se utiliza las clases Thread y Socket provistas por la cátedra: https://github.com/eldipa/hands-on-threads
https://github.com/eldipa/sockets-en-cpp

### Bibliografía

Para garantizar el correcto uso de las bibliotecas SDL2 y Qt, se utilizó como principal referencia la documentación oficial de ambas. Estas fuentes proporcionaron información detallada y ejemplos claros que resultaron fundamentales para comprender las funcionalidades disponibles y aplicarlas de manera adecuada en el proyecto.

Además, los tutoriales de YouTube fueron un recurso complementario fundamental, ya que ofrecieron explicaciones visuales y ejemplos prácticos que facilitaron la comprensión de conceptos más complejos. Por su parte, StackOverflow fue de gran ayuda para resolver problemas puntuales, al ofrecer soluciones específicas basadas en las experiencias de otros desarrolladores.
