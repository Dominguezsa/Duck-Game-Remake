#ifndef SERVER_PATO_H
#define SERVER_CLASS_H

#include <cstdint>

#include "../common/common_arma.h"
#include "../common/common_posicion.h"

class Pato {
public:
    uint8_t pato_id;
    uint8_t puntos_de_vida;
    uint8_t mira_hacia;  // 0 -> izquierda, 1 -> derecha, 2 -> arriba, 3 -> abajo
    bool esta_vivo;
    bool esta_corriendo;
    bool esta_saltando;
    bool esta_agachado;
    bool esta_disparando;
    bool tiene_casco;
    bool tiene_armadura;
    Posicion posicion;
    Arma arma;


    explicit Pato(uint8_t id, uint8_t vida, bool mira_hacia, Posicion pos, const Arma& arma);
    void mover_hacia(uint8_t direccion);  // 0: izquierda, 1: derecha (hacia arriba seria un salto y
                                          // hacia abajo se agacha)

    void saltar(bool activar);  // recibe activar para saber si se activa o desactiva el salto (y no
                                // implementar 2 mensajes)

    void agacharse(bool activar);

    void disparar(bool activar);

    void recibir_danio(uint8_t danio);

    void agarrar_arma(const Arma& arma);

    void soltar_arma();
};

#endif  // SERVER_PATO_H
