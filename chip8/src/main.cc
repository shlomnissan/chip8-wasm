#include <iostream>
#include <emscripten.h>

#include "emulator.h"

void do_error(const string& kErrorMessage) {
    std::cerr << "ERROR: " << kErrorMessage << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        do_error("Usage: chip8 ${FILENAME}");
        return EXIT_FAILURE;
    }

    Emulator chip8;
    const string kRom = argv[1];

    if (chip8.Boot()) {
        if (chip8.LoadRom(kRom)) {
            chip8.Start();
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