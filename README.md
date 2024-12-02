# Duck Game

## Cómo correr el proyecto

Para instalar el juego, correr en el directorio raiz de este proyecto:
```bash

sudo bash ./install.sh

```

Que corre los scripts para descargar las dependencias necesarias

## Ejecución

Dentro del repositorio principal, para ejecutar cliente y server 

```bash
./taller_server <puerto>
```

```bash
./taller_client <hostname> <puerto>
```

Para correr el editor se necesita el comando de superusuario 'sudo', para que pueda guardar los mapas en el directorio /var/duck_game

```bash
sudo ./taller_editor
```

-------------------------------------------------------------
### IMPORTANTE

No probar las siguientes armas porque no estan implementadas y crashean el juego: granada, banana,

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
