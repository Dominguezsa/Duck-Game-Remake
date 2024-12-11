# Manual de Usuario

## Cómo correr el proyecto

Para instalar el juego, correr dentro de la carpeta "/instalador" el script install.sh de la siguiente forma:

```bash
chmod +x install.sh
```

```bash
sh install.sh
```

Que corre los scripts para descargar las dependencias necesarias

## Ejecución

Como se agregan los ejecutables de cliente, server y el editor de mapa a "usr/bin", se pueden correr desde cualquier directorio.

```bash
duck_game_server <puerto>
```

```bash
duck_game <hostname> <puerto>
```

Para correr el editor se necesita el comando de superusuario 'sudo', para que pueda guardar los mapas en el directorio /var/duck_game

```bash
sudo map_editor
```

-------------------------------------------------------------
### IMPORTANTE

No probar las siguientes armas porque no estan implementadas y crashean el juego: granada, banana,
