//
// Created by Shlomi Nissan on 12/16/19.
//

#include <fstream>
#include <vector>
#include <emscripten.h>

#include "emulator.h"

const auto kCyclePerSecond = 10;  // Arbitrary, works best for most roms

bool Emulator::Boot() {
    return window.Initialize();
}

bool Emulator::LoadRom(const string& kFile) {
    std::ifstream rom (kFile);
    if (!rom) return false;

    rom.seekg(0, std::ios::end);
    const int kSize = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // Read rom data into a temporary buffer
    std::vector<char> buffer;
    buffer.reserve(kSize);
    buffer.insert(buffer.begin(),
            std::istreambuf_iterator<char>(rom),
            std::istreambuf_iterator<char>());

    chip8.SaveRom(buffer.data(), buffer.size());

    return true;
}

void Emulator::Run() {
    chip8.UpdateTimers();
    window.PollEvents(input);
    for (int i = 0; i < kCyclePerSecond; ++i) {
        chip8.Cycle();
    }
    window.Draw(display);
    
}
