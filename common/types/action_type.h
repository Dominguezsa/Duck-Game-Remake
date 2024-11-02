#ifndef ACTION_TYPE_H
#define ACTION_TYPE_H

#include <cstdint>

enum ActionType : const uint8_t {
    MOVE_RIGHT_KEY_DOWN = 0x01,
    MOVE_RIGHT_KEY_UP,
    MOVE_LEFT_KEY_DOWN,
    MOVE_LEFT_KEY_UP,
    JUMP_KEY_DOWN,
    JUMP_KEY_UP,
    SHOOT_KEY_DOWN,
    SHOOT_KEY_UP
};

// const uint8_t MOVE_RIGHT_KEY_DOWN = 0x01;
// const uint8_t MOVE_RIGHT_KEY_UP = 0x02;
// const uint8_t MOVE_LEFT_KEY_DOWN = 0x03;
// const uint8_t MOVE_LEFT_KEY_UP = 0x04;
// const uint8_t JUMP_KEY_DOWN = 0x05;
// const uint8_t JUMP_KEY_UP = 0x06;
// const uint8_t SHOOT_KEY_DOWN = 0x07;
// const uint8_t SHOOT_KEY_UP = 0x08;

#endif  // ACTION_TYPE_H
