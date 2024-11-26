#include "bullet.h"

#include <cmath>

Bullet::Bullet(uint8_t id, float x, float y, float angle, float speed, float time, bool going_right,
               uint8_t damage):
        id(id),
        x(x),
        y(y),
        angle(angle),
        speed(speed),
        time(time),
        damage(damage),
        going_right(going_right) {}

Bullet::Bullet(const Bullet& other):
        id(other.id),
        x(other.x),
        y(other.y),
        angle(other.angle),
        speed(other.speed),
        time(other.time),
        damage(other.damage),
        going_right(other.going_right) {}

void Bullet::move() {
    // Update the position of the bullet based on its speed and angle
    float delta_x = speed * cos(angle);
    float delta_y = speed * sin(angle);

    if (going_right) {
        x += delta_x;
    } else {
        x -= delta_x;
    }
    y += delta_y;

    // Update the time
    time += 1.0f;  // Assuming each move call represents a time step of 1.0f
}
