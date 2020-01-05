//
// Created by shlominissan on 12/19/19.
//

#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <array>
#include <cstring>

class Display {
public:
    const int width() { return kWidth; }
    const int height() { return kHeight; }
    const int scale() { return kScale; }

    uint8_t& operator[](int index) {
        if (index > display.size()) {
            throw std::out_of_range("display[] : index out of range");
        }
        return display[index];
    }

    void ClearScreen() {
        std::memset(display.data(), 0, kWidth * kHeight);
    }

    static Display& Instance() {
        static Display INSTANCE;
        return INSTANCE;
    }
private:
    static const int kWidth = 64;
    static const int kHeight = 32;
    static const int kScale = 10;

    std::array<uint8_t, kWidth * kHeight> display;

    Display(): display() {}
};

#endif //CHIP8_DISPLAY_H
