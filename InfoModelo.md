## ProtocoloDelServidor: El protocolo del servidor se crea con un socket y un id (que corresopnde con un cliente) asi este no se lo tiene que enviar continuamente.

Cada accion del cliente, saltar, moverse a la izquierda, moverse a la derecha, disparar, etc, se envia al servidor con un unico caracter en forma de uint8. El protocolo recibe esta accion, y esta listo para enviarlo a la unica gameloopQueue a través del mensaje_gameloop que tiene como atributos el id del jugador y la accion a realizar.

¿Qué es lo que le envia a los cliente?
Primero un uint8 de cuantos Patos les envia. Luego, posicionPato1_x, posicionPato1_y, esta_vivo, esta_corriendo, esta_saltando, esta_agachado, esta_disparando, tiene_casco, tiene_armadura, arma, posicionPato2_x, ...

Un ejemplo podria ser:

0x02 0x00000000 0x00000000 0x01  0x00  0x00  0x00  0x00  0x00   0x00   0x03        0x00000000  0x00000000 0x01...
cant   pos_x      pos_y    vivo  corr  salt agach  disp  casco  armad  arma          pos_x        pos_y  vivo ...



