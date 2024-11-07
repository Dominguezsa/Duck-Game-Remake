## ProtocoloDelServidor: El protocolo del servidor se crea con un socket y un id (que corresopnde con un cliente) asi este no se lo tiene que enviar continuamente.

Cada accion del cliente, saltar, moverse a la izquierda, moverse a la derecha, disparar, etc, se envia al servidor con un unico caracter en forma de uint8. El protocolo recibe esta accion, y esta listo para enviarlo a la unica gameloopQueue a través del mensaje_gameloop que tiene como atributos el id del jugador y la accion a realizar.

¿Qué es lo que le envia a los cliente?
Primero un uint8 de cuantos Patos les envia. Luego, idPato1, posicionPato1_x, posicionPato1_y, esta_vivo, esta_corriendo, esta_saltando, esta_agachado, esta_disparando, tiene_casco, tiene_armadura, arma, posicionPato2_x, ...

OBS: ¿Para que necesitamos el id? De otra forma el cliente no podrá saber a qué patos corresponden cada uno de los estados recibidos desde el servidor.
Entonces, se asume que el id de los patos es idéntico para un mismo pato tanto del lado del servidor como del lado del server.

Cada uno de los campos enviados a través del protocolo coincide en valor y orden con los del struct DuckState, definido en common/types/duck_state.h

La idea es que cada protocolo lea y envie (si es el del Server) o reciba y guarde (si es el Cliente) la info descripta a continuacion sobre el struct DuckState.

Un ejemplo podria ser:

0x02 0x00000000 0x00000000 0x01 0x01  0x00  0x00  0x00  0x00  0x00   0x00   0x03        0x00000000  0x00000000 0x01...
cant   pos_x      pos_y    vivo  corr  salt agach  disp  casco  armad  arma          pos_x        pos_y  vivo ...

La clase Duck sabe como utilizar un DuckState struct (mediante los métodos get_state(...) y update_state(...))

Podemos cargar el estado actual de un pato en un DuckStruct haciendo:

duck.get_state(duckState) <- por ejemplo, justo antes de enviar la info desde el ServerProtocol

Podemos actualizar el estado de un DUck segun un DuckState haciendo:

duck.update_state(duckState) <- por ejemplo, justo despues de recibir un estado en el ClientProtocol.

protocolo del cliente al server:
(Constantes de este protocolo definidas en common/types/action_type.h)

0x01: mover_a_derecha_key_down
0x02: mover_a_derecha_key_up
0x03: mover_a_izquierda_key_down
0x04: mover_a_izquierda_key_up
0x05: saltar_key_down
0x06: saltar_key_up
0x07: disparar_key_down
0x08: disparar_key_up

------------------------------------------------

# Protocolos para los mensajes del lobby:

Los siguientes mensajes son descriptos en orden.

- Si se quiere crear una partida:

(1) client -> server

   'C'    len_player_name        player_name
(1 byte)     (2 bytes)     (len_player_name bytes)

El caracter 'C' haciendo referencia a la acción CREATE.


(2) server -> client

cant_de_mapas_existentes  len_map_1_name  map1_name  
       (2 bytes)            (2 bytes)    (n1 bytes)

... len_map_m_name     map_m_name
       (2 bytes)      (n_m bytes)

Los mapas de los mensajes pueden ser o bien los que existen por default, o bien los que fueron creados por otras personas. El cliente tras recibir la info de todos los que existen va a elegir en cual crear su partida (indicandolo como en el proximo tipo de msg)


(3) client -> server

  'C'     number_of_players  len_match_name  match_name
(1 byte)      (1 byte)         (2 bytes)     (n bytes)


(4) server -> client

byte_de_confirmacion (1 byte)

0x01 si la partida de creó con éxito.

0x00 si no se creó la partida. Un motivo podria ser por ej que se le envió un nombre que coincide con el de otra partida existente.
Si recibimos 0x00 deberiamos volver a enviar un msg de tipo (3) es podria estar en un while, (while not partida_creada)


- Para unirse a una partida:

(1) client -> server

Similar al msg (1) de CREATE, pero en lugar de 'C' enviar una 'J', que hace referencia a la accion JOIN.


(2) server -> client

cant_de_partidas_disponibles  len_match1_name
         (n bytes)               (2 bytes)

match1_name ... len_match_m_name  match_m_name 
(n1 bytes)         (2 bytes)       (n_m bytes)

Las partidas disponibles son todas aquellas activas en ese momento y que admitan recibir jugadores.


(3) client -> server

len_selected_match_name  selected_match_name
     (2 bytes)                (n bytes)


(4) server -> client

byte_de_confirmacion (1 byte)

0x01 si nos unimos con éxito.

0x00 si no. Un motivo podria ser por ej que durante el tiempo que se tardo en intentar unirnos ya se unio alguien mas y la partida indicada antes ya esta llena.

Si recibimos 0x00 deberiamos volver a enviar un msg de tipo (2) con las partidas disponibles al momento actual. Esta logica tambien posiblemente vaya a estar en un while.