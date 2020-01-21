#include <iostream>
#include <emscripten.h>

#include "emulator.h"

Emulator chip8;

void do_error(const string& kErrorMessage) {
    std::cerr << "ERROR: " << kErrorMessage << '\n';
}

void main_loop() {
    chip8.Run();
}

int main() {
    const string kRom = "../roms/INVADERS.ch8";

    if (chip8.Boot()) {
        if (chip8.LoadRom(kRom)) {
            emscripten_set_main_loop(main_loop,/* fps */ -1, /* infinite loop */ true);
        } else {
            do_error("Failed to open " + kRom);
            return EXIT_FAILURE;
        }
    } else {
        do_error("Failed to boot emulator");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
