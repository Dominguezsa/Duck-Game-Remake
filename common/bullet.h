#ifndef BULLET_H
#define BULLET_H

#include <cstdint>


class Bullet {
public:
    uint8_t id;  // esto es de que arma es la bala porque cambia la foto
    float x;
    float y;
    float angle;
    float speed;  // se tiene una ecuacion de la bala que es casi una linea recta entonces en cada
                  // itereacion se va actualizando su posicion, si fuese una granada es una
                  // parabola, etc

    float time;  // posicion en el tiempo de la bala

    bool going_right;  // si la bala va hacia la derecha o izquierda asi en la ecuacion se le suma o
                       // resta la velocidad
    uint8_t damage;

    Bullet(uint8_t id, float x, float y, float angle, float speed, float time, bool going_right,
           uint8_t damage);
    Bullet(const Bullet& other);
    void move();
};


#endif  // BULLET_H
