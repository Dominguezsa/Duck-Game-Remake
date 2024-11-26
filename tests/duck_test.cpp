#include "../common/duck.cpp"

#include <cassert>
#include <iostream>

#include "../common/common_position.h"
#include "../common/common_weapon.h"

// TODO: testear el protocolo
void test_pato() {
    Duck pato(1, 100, 1, Position(0, 0), Weapon("ak47", 10));

    pato.move_to(0);

    assert(pato.position.x == -1);
    assert(pato.position.y == 0);
    assert(pato.duck_id == 1);
    assert(pato.life_points == 100);
    assert(pato.looking == 0);
    assert(pato.is_alive == true);
    assert(pato.weapon.name == "ak47");
    assert(pato.weapon.ammo == 10);
    std::cout << "Test básico de pato pasó" << std::endl;
}


int main() {
    test_pato();
    std::cout << std::endl;
    std::cout << ":) :) :) :) :) :)  Todas las pruebas pasaron, Genial :) :) :) :) :) :) :) :)"
              << std::endl;
    return 0;
}
