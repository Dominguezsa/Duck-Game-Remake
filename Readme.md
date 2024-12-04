# Duck Game

El objetivo de este proyecto es hacer un remake del clásico juego de arcade "Duck Game", en el cual los jugadores controlan a un pato dentro de un multijugador, cuyo objetivo es mantenerse con vida y deshacerse de los demás jugadores. 

## Cómo correr el proyecto

Para instalar el juego, correr en el directorio raiz de este proyecto:
```bash

sudo bash ./install.sh

```

Que corre los scripts para descargar las dependencias necesarias

## Ejecución

Dentro del repositorio principal, para ejecutar cliente y server 

```bash
./duck_game_server <puerto>
```

```bash
./duck_game <hostname> <puerto>
```

Para correr el editor se necesita el comando de superusuario 'sudo', para que pueda guardar los mapas en el directorio /var/duck_game

```bash
sudo ./map_editor
```

-------------------------------------------------------------
### IMPORTANTE

No probar las siguientes armas porque no estan implementadas y crashean el juego: granada, banana,
