#include "bullet.h"

#include <cmath>

Bullet::Bullet(uint8_t id, float x, float y, int angle, float speed, float time, bool going_right,
               uint8_t damage, uint8_t duck_how_shot):
        id(id),
        x(x),
        y(y),
        angle(angle),
        speed(speed),
        time(time),
        going_right(going_right),
        damage(damage),
        duck_how_shot(duck_how_shot) {}

Bullet::Bullet(const Bullet& other):
        id(other.id),
        x(other.x),
        y(other.y),
        angle(other.angle),
        speed(other.speed),
        time(other.time),
        going_right(other.going_right),
        damage(other.damage),
        duck_how_shot(other.duck_how_shot) {}

Bullet::Bullet(uint8_t id, float x, float y, bool going_right, int angle):
        id(id),
        x(x),
        y(y),
        angle(angle),
        speed(0.0f),
        time(0.0f),
        going_right(going_right),
        damage(0),
        duck_how_shot(0) {}


Bullet::Bullet():
        id(0),
        x(0.0f),
        y(0.0f),
        angle(0),
        speed(0.0f),
        time(0.0f),
        going_right(false),
        damage(0),
        duck_how_shot(0) {}

void Bullet::move() {
    // Update the position of the bullet based on its speed and angle
    // If i use this, the deltas are always positive and the bullet always goes to the right
    // float delta_x = speed * cos(angle);
    // float delta_y = speed * sin(angle);

    float delta_x = speed;
    // Para pasar de grados a radianes porque necesito los grados en el cliente
    float delta_y = speed * sin(angle * (M_PI) / 180.0f);

    if (going_right) {
        x += delta_x;
    } else {
        x -= delta_x;
    }
    y += delta_y;

    // Update the time
    time += 1.0f;  // Assuming each move call represents a time step of 1.0f
}

void Bullet::update(const Bullet& other) {
    id = other.id;
    x = other.x;
    y = other.y;
    angle = other.angle;
    speed = other.speed;
    time = other.time;
    going_right = other.going_right;
    damage = other.damage;
    duck_how_shot = other.duck_how_shot;
}

void Bullet::addDuckHowShot(uint8_t duck) { duck_how_shot = duck; }
