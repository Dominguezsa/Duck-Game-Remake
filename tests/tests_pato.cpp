#include <cassert>
#include <iostream>

#include "../common/common_arma.h"
#include "../common/common_posicion.h"
#include "../server/server_pato.cpp"

void test_pato() {
    Pato pato(1, 100, 1, Posicion(0, 0), Arma("ak47", 10));

    pato.mover_hacia(0);

    assert(pato.posicion.x == -1);
    assert(pato.posicion.y == 0);
    assert(pato.pato_id == 1);
    assert(pato.puntos_de_vida == 100);
    assert(pato.mira_hacia == 0);
    assert(pato.esta_vivo == true);
    assert(pato.arma.nombre == "ak47");
    assert(pato.arma.municion == 10);
    std::cout << "Test básico de pato pasó" << std::endl;
}


int main() {
    test_pato();
    std::cout << std::endl;
    std::cout << ":) :) :) :) :) :)  Todas las pruebas pasaron, Genial :) :) :) :) :) :) :) :)"
              << std::endl;
    return 0;
}
