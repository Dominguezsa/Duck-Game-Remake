#include "server_pato.h"

Pato::Pato(uint8_t id, uint8_t vida, bool mira_hacia, Posicion pos, Arma arma)
    : pato_id(id),
      puntos_de_vida(vida),
      mira_hacia(mira_hacia),
      esta_vivo(true),
      esta_corriendo(false),
      esta_saltando(false),
      esta_agachado(false),
      esta_disparando(false),
      tiene_casco(false),
      tiene_armadura(false),
      posicion(pos),
      arma(arma) {}

void Pato::mover_hacia(uint8_t direccion) {
    if (direccion == 0) {
        posicion.x -= 1;
        mira_hacia = 0;
    } else if (direccion == 1) {
        posicion.x += 1;
        mira_hacia = 1;
    }
}

void Pato::saltar(bool activar) {
    if (activar) {
        esta_saltando = true;
    } else {
        esta_saltando = false;
    }
}

void Pato::agacharse(bool activar) {
    if (activar) {
        esta_agachado = true;
    } else {
        esta_agachado = false;
    }
}

void Pato::disparar(bool activar) {
    if (activar) {
        esta_disparando = true;
    } else {
        esta_disparando = false;
    }
}

void Pato::recibir_danio(uint8_t danio) {
    int puntos_de_vida_con_signo = puntos_de_vida;
    if (puntos_de_vida_con_signo - danio <= 0) {
        puntos_de_vida = 0;
        esta_vivo = false;
    } else {
        puntos_de_vida -= danio;
    }
}

void Pato::agarrar_arma(Arma arma) {
    this->arma = arma;
}

void Pato::soltar_arma() {
    this->arma = Arma("", 0);
}
