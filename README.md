# Duck Game

## Cómo correr el proyecto

Para instalar el juego, correr
```bash

sudo bash ./install.sh

```

Que corre los scripts para descargar las dependencias necesarias y mueve los binarios a 
/usr/bin y los recursos del juego a /var/duck_game/data

## Ejecución

Estando dentro del directorio /usr/bin/, correr:

```bash
./taller_server <puerto>
```
para el server, y:

```bash
./taller_client <hostname> <puerto>
```

para correr los clientes.

-------------------------------------------------------------

**Importante:** el primer commit de este repositorio tiene el setup
básico para el TP Final que incluye la instalación de la lib
`libSDL2pp` (el wrapper de C++).

El resto de los commits son a modo de ejemplo de como se pueden
agregar mas código fuente al proyecto.

Este ejemplo **no** incluye instalarse la librería `SDL2` ni tampoco
instala otras librerías que puedan ser necesarias ni tampoco un
instalador (aunque **si** incluye Google Tests)

**Se deben agregar las librerias necesarias y el instalador.**

También el ejemplo usa una estructura de carpetas muy simple:

```
client/
server/
editor/
common/
```

Bien se puede mejorar (cambiando el cmakefile) agregando mas
sub-carpetas.

Asi tambien **deben** ser cambiados los *targets* del cmake (`taller_client`,
`taller_server`, ...) por nombres mas acordes al TP que se este
haciendo.

Tambien, por default solo se compila una version *debug* sin
optimizar. Si se quiere compilar binarios optimizados
(lo que cmake llama *release*) se puede, solo hay modificar
cmake.

Aprender del ejemplo para saber como extenderlo!

**Importante:** este repositorio **no** incluye pre-commits hooks,
ni scripts adicionales (como correr valgrind).

**Eso esta a cargo de los estudiantes,** tal como fue mostrado
en los tps individuales, recaps y hands-on.
