//
// Created by Shlomi Nissan on 12/16/19.
//

#include <fstream>
#include <vector>
#include <chrono>

#include "emulator.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

const float kDelayCycle = 2.5f;     // Works best for most roms
const float kDelayTimers = 16.66f;  // 1000ms / 60 = 16.66

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

void Emulator::Start() {
    auto tick_cycle = high_resolution_clock::now();
    auto tick_timers = tick_cycle;

    while (window.get_running()) {
        auto current_time = high_resolution_clock::now();
        auto dt_cycle = duration<float, milliseconds::period>(current_time - tick_cycle).count();
        auto dt_timers = duration<float, milliseconds::period>(current_time - tick_timers).count();

        if (dt_cycle > kDelayCycle) {
            tick_cycle = current_time;
            window.PollEvents(input);
            chip8.Cycle();
            window.Draw(display);
        }

        // Delay/sound timers are 60hz
        if (dt_timers > kDelayTimers) {
            tick_timers = current_time;
            chip8.UpdateTimers();
        }
    }
}