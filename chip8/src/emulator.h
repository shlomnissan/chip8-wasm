//
// Created by Shlomi Nissan on 12/16/19.
//

#ifndef CHIP8_EMULATOR_H
#define CHIP8_EMULATOR_H

#include <string>

#include "window.h"
#include "chip8.h"

using std::string;

class Emulator {
public:
    bool Boot();
    bool LoadRom(const string& kFile);
    void Start();

    Emulator(): display(Display::Instance()),
                input(Input::Instance()),
                window(
                    display.width() * display.scale(),
                    display.height() * display.scale()
                ) {}
private:
    Display& display;
    Input& input;
    Window window;
    Chip8 chip8;
};


#endif //CHIP8_EMULATOR_H
