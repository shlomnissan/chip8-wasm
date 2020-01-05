//
// Created by Shlomi Nissan on 12/31/19.
//

#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#include <array>

/*
    ------------------------------------------------
    |   Chip 8 Keyboard    |     Keyboard Map      |
    ------------------------------------------------
    |   1    2    3    C   |    1    2    3    4   |
    |   4    5    6    D   |    Q    W    E    R   |
    |   7    8    9    E   |    A    S    D    F   |
    |   A    0    B    F   |    Z    X    C    V   |
    ------------------------------------------------
*/

class Input {
public:
    uint8_t& operator[](uint8_t index) {
        if (index > keys.size()) {
            throw std::out_of_range("keys[] : index out of range");
        }
        return keys[index];
    }

    bool IsPressed(uint8_t key) {
        if (key > keys.size()) {
            throw std::out_of_range("IsPressed : keys[] : index out of range");
        }
        return keys[kKeyMap[key]] == 1;
    }

    uint8_t size() {
        return 0x10;
    }

    static Input& Instance() {
        static Input INSTANCE;
        return INSTANCE;
    }
private:
    std::array<uint8_t, 256> keys;

    std::array<uint8_t, 0x10> kKeyMap = {
        0x78, // X
        0x31, // 1
        0x32, // 2
        0x33, // 3
        0x71, // Q
        0x77, // W
        0x65, // E
        0x61, // A
        0x73, // S
        0x64, // D
        0x7a, // Z
        0x63, // C
        0x34, // 4
        0x72, // R
        0x66, // F
        0x76, // V
    };
};


#endif //CHIP8_INPUT_H
