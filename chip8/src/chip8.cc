//
// Created by Shlomi Nissan on 12/17/19.
//

#include "chip8.h"
#include "interpreter.h"

#include <iostream>
#include <cstring>

using std::memset;
using std::memcpy;

constexpr std::array<uint8_t, 0x50> kSprites {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

constexpr int kStartAddress = 0x200;

constexpr int kOp1 = 0x1;  // Default operations set
constexpr int kOp0 = 0x0;
constexpr int kOp8 = 0x8;
constexpr int kOpE = 0xE;
constexpr int kOpF = 0xF;

Chip8::Chip8(): memory({0}),
                registers({0}),
                stack({0}),
                operations(),
                display(Display::Instance()),
                input(Input::Instance()),
                t_delay(0),
                t_sound(0),
                I(0),
                pc(kStartAddress),
                sp(0),
                opcode(0),
                rand(0, 255) {
    // Store sprites data at the beginning of the memory
    memcpy(memory.data(), kSprites.data(), kSprites.size());

    BindOperations();
}

void Chip8::SaveRom(const void *source) {
    // 0x200 (512) Start of most Chip-8 programs
    memcpy(memory.data() + kStartAddress, source, memory.size());
}

void Chip8::Reset() {
    // Clear out rom data from memory
    memset(memory.data() + kStartAddress, 0, memory.size() - kStartAddress);
    registers = {0};
    stack = {0};
    I = 0;
    pc = 0x200;
    sp = 0;
    t_delay = 0;
    t_sound = 0;
}

void Chip8::Cycle() {
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc + 1];

    // Increment PC before execution
    pc += 2;

    try {
        switch ((opcode & 0xF000u) >> 12u) {
            case kOp0:
                operations[kOp0][(opcode & 0x000Fu)]();
                break;
            case kOp8:
                operations[kOp8][(opcode & 0x000Fu)]();
                break;
            case kOpE:
                operations[kOpE][(opcode & 0x000Fu)]();
                break;
            case kOpF:
                operations[kOpF][(opcode & 0x00FFu)]();
                break;
            default: {
                operations[kOp1][((opcode & 0xF000u) >> 12u)]();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n'
                << "Failed to execute opcode: "
                << std::hex << opcode << '\n';
    }
}

void Chip8::UpdateTimers() {
    // Decrement the delay timer if it's been set
    if (t_delay > 0) {
        --t_delay;
    }

    // Decrement the sound timer if it's been set
    if (t_sound > 0) {
        --t_sound;
    }
}

void Chip8::BindOperations() {
    Interpreter d(*this);

    operations[kOp0][0x00] = std::bind(&Interpreter::OP_00E0, d);
    operations[kOp0][0x0E] = std::bind(&Interpreter::OP_00EE, d);
    operations[kOp1][0x01] = std::bind(&Interpreter::OP_1nnn, d);
    operations[kOp1][0x02] = std::bind(&Interpreter::OP_2nnn, d);
    operations[kOp1][0x03] = std::bind(&Interpreter::OP_3xkk, d);
    operations[kOp1][0x04] = std::bind(&Interpreter::OP_4xkk, d);
    operations[kOp1][0x05] = std::bind(&Interpreter::OP_5xy0, d);
    operations[kOp1][0x06] = std::bind(&Interpreter::OP_6xkk, d);
    operations[kOp1][0x07] = std::bind(&Interpreter::OP_7xkk, d);
    operations[kOp1][0x09] = std::bind(&Interpreter::OP_9xy0, d);
    operations[kOp1][0x0A] = std::bind(&Interpreter::OP_Annn, d);
    operations[kOp1][0x0B] = std::bind(&Interpreter::OP_Bnnn, d);
    operations[kOp1][0x0C] = std::bind(&Interpreter::OP_Cxkk, d);
    operations[kOp1][0x0D] = std::bind(&Interpreter::OP_Dxyn, d);
    operations[kOp8][0x00] = std::bind(&Interpreter::OP_8xy0, d);
    operations[kOp8][0x01] = std::bind(&Interpreter::OP_8xy1, d);
    operations[kOp8][0x02] = std::bind(&Interpreter::OP_8xy2, d);
    operations[kOp8][0x03] = std::bind(&Interpreter::OP_8xy3, d);
    operations[kOp8][0x04] = std::bind(&Interpreter::OP_8xy4, d);
    operations[kOp8][0x05] = std::bind(&Interpreter::OP_8xy5, d);
    operations[kOp8][0x06] = std::bind(&Interpreter::OP_8xy6, d);
    operations[kOp8][0x07] = std::bind(&Interpreter::OP_8xy7, d);
    operations[kOp8][0x0E] = std::bind(&Interpreter::OP_8xyE, d);
    operations[kOpE][0x01] = std::bind(&Interpreter::OP_ExA1, d);
    operations[kOpE][0x0E] = std::bind(&Interpreter::OP_Ex9E, d);
    operations[kOpF][0x07] = std::bind(&Interpreter::OP_Fx07, d);
    operations[kOpF][0x0A] = std::bind(&Interpreter::OP_Fx0A, d);
    operations[kOpF][0x15] = std::bind(&Interpreter::OP_Fx15, d);
    operations[kOpF][0x18] = std::bind(&Interpreter::OP_Fx18, d);
    operations[kOpF][0x1E] = std::bind(&Interpreter::OP_Fx1E, d);
    operations[kOpF][0x29] = std::bind(&Interpreter::OP_Fx29, d);
    operations[kOpF][0x33] = std::bind(&Interpreter::OP_Fx33, d);
    operations[kOpF][0x55] = std::bind(&Interpreter::OP_Fx55, d);
    operations[kOpF][0x65] = std::bind(&Interpreter::OP_Fx65, d);
}
