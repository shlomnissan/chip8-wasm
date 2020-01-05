//
// Created by Shlomi Nissan on 12/20/19.
//

#include "interpreter.h"

//region Helpers

uint8_t Interpreter::getX() {
    return (c8.opcode & 0x0F00u) >> 8u;
}

uint8_t Interpreter::getY() {
    return (c8.opcode & 0x00F0u) >> 4u;
}

//endregion

// Clear the screen
void Interpreter::OP_00E0() {
    c8.display.ClearScreen();
}

// Return from a subroutine
void Interpreter::OP_00EE() {
    c8.pc = c8.stack[--c8.sp];
}

// Jump to location nnn
void Interpreter::OP_1nnn() {
    uint16_t address = c8.opcode & 0x0FFFu;
    c8.pc = address;
}

// Call subroutine at nnn
void Interpreter::OP_2nnn() {
    uint16_t address = c8.opcode & 0x0FFFu;
    c8.stack[c8.sp++] = c8.pc;
    c8.pc = address;
}

// Skip next instruction of Vx == kk
void Interpreter::OP_3xkk() {
    uint8_t Vx = getX();
    uint8_t byte = c8.opcode & 0x00FFu;
    if (c8.registers[Vx] == byte) {
        c8.pc += 2;
    }
}

// Skip next instruction if Vx != kk
void Interpreter::OP_4xkk() {
    uint8_t Vx = getX();
    uint8_t byte = c8.opcode & 0x00FFu;
    if (c8.registers[Vx] != byte) {
        c8.pc += 2;
    }
}

// Skip next instruction if Vx = Vy
void Interpreter::OP_5xy0() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    if (c8.registers[Vx] == c8.registers[Vy]) {
        c8.pc += 2;
    }
}

// Set Vx = kk
void Interpreter::OP_6xkk() {
    uint8_t Vx = getX();
    uint8_t byte = c8.opcode & 0x00FFu;
    c8.registers[Vx] = byte;
}

// Set Vx = Vx + kk
void Interpreter::OP_7xkk() {
    uint8_t Vx = getX();
    uint8_t byte = c8.opcode & 0x00FFu;
    c8.registers[Vx] += byte;
}

// Set Vx = Vy
void Interpreter::OP_8xy0() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[Vx] = c8.registers[Vy];
}

// Set Vx = Vx OR Vy
void Interpreter::OP_8xy1() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[Vx] |= c8.registers[Vy];
}

// Set Vx = Vx AND Vy
void Interpreter::OP_8xy2() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[Vx] &= c8.registers[Vy];
}

// Set Vx = Vx XOR Vy
void Interpreter::OP_8xy3() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[Vx] ^= c8.registers[Vy];
}

// Set Vx = Vx + Vy, set VF = carry
void Interpreter::OP_8xy4() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    uint16_t sum = c8.registers[Vx] + c8.registers[Vy];
    c8.registers[0xF] = (sum > 255) ? 1 : 0;
    c8.registers[Vx] = sum & 0xFFu;
}

// Set Vx = Vx - Vy, set VF = NOT borrow
void Interpreter::OP_8xy5() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[0xF] = (c8.registers[Vx] > c8.registers[Vy]) ? 1 : 0;
    c8.registers[Vx] -= c8.registers[Vy];
}

// Set Vx = Vx SHR 1
void Interpreter::OP_8xy6() {
    uint8_t Vx = getX();
    c8.registers[0xF] = (c8.registers[Vx] & 0x1u);
    c8.registers[Vx] >>= 1;
}

// Set Vx = Vy - Vx, set VF = NOT borrow
void Interpreter::OP_8xy7() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    c8.registers[0xF] = (c8.registers[Vy] > c8.registers[Vx]) ? 1 : 0;
    c8.registers[Vx] = c8.registers[Vy] - c8.registers[Vx];
}

// Set Vx = Vx SHL 1
void Interpreter::OP_8xyE() {
    uint8_t Vx = getX();
    c8.registers[0xF] = (c8.registers[Vx] & 0x80u) >> 7u;
    c8.registers[Vx] <<= 1;
}

// Skip next instruction if Vx != Vy
void Interpreter::OP_9xy0() {
    uint8_t Vx = getX();
    uint8_t Vy = getY();
    if (c8.registers[Vx] != c8.registers[Vy]) {
        c8.pc += 2;
    }
}

// Set I = nnn
void Interpreter::OP_Annn() {
    uint16_t address = c8.opcode & 0x0FFFu;
    c8.I = address;
}

// Jump to location nnn + V0
void Interpreter::OP_Bnnn() {
    uint16_t address = c8.opcode & 0x0FFFu;
    c8.pc = c8.registers[0] + address;
}

// Set Vx = random byte AND kk
void Interpreter::OP_Cxkk() {
    uint8_t Vx = getX();
    uint8_t byte = c8.opcode & 0x00FFu;
    c8.registers[Vx] = c8.rand() & byte;
}

// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void Interpreter::OP_Dxyn() {
    uint16_t Vx = c8.registers[getX()];
    uint16_t Vy = c8.registers[getY()];
    uint16_t height = c8.opcode & 0x000F;
    uint16_t pixel;

    c8.registers[0xF] = 0;

    for (size_t y = 0; y < height; y++) {
        pixel = c8.memory[c8.I + y];
        for (size_t x = 0; x < 8; x++) {
            if (pixel & (0x80u >> x)) {
                uint32_t loc = x + Vx + (y + Vy) * c8.display.width();
                if (c8.display[loc]) {
                    c8.registers[0xF] = 1;
                }
                c8.display[loc] ^= 1;
            }
        }
    }
}

// Skip next instruction if key with the value of Vx is pressed
void Interpreter::OP_Ex9E() {
    uint8_t Vx = getX();
    if (c8.input.IsPressed(c8.registers[Vx])) {
        c8.pc += 2;
    }
}

// Skip next instruction if key with the value of Vx is not pressed
void Interpreter::OP_ExA1() {
    uint8_t Vx = getX();
    if (!c8.input.IsPressed(c8.registers[Vx])) {
        c8.pc += 2;
    }
}

// Set Vx = delay timer value
void Interpreter::OP_Fx07() {
    uint8_t Vx = getX();
    c8.registers[Vx] = c8.t_delay;
}

// Wait for a key press, store the value of the key in Vx
void Interpreter::OP_Fx0A() {
    uint8_t Vx = getX();
    for (uint8_t i = 0; i < c8.input.size(); i++) {
        if (c8.input.IsPressed(i)) {
            c8.registers[Vx] = i;
        }
    }
}

// Set delay timer = Vx
void Interpreter::OP_Fx15() {
    uint8_t Vx = getX();
    c8.t_delay = c8.registers[Vx];
}

// Set sound timer = Vx
void Interpreter::OP_Fx18() {
    uint8_t Vx = getX();
    c8.t_sound = c8.registers[Vx];
}

// Set I = I + Vx
void Interpreter::OP_Fx1E() {
    uint8_t Vx = getX();
    c8.I += c8.registers[Vx];
}

// Set I = location of sprite for digit Vx
void Interpreter::OP_Fx29() {
    uint8_t Vx = getX();
    c8.I = c8.registers[Vx] * 5; // sprites start at 0x0
}

// Store BCD representation of Vx in memory locations I, I+1, and I+2
void Interpreter::OP_Fx33() {
    uint8_t Vx = getX();
    c8.memory[c8.I] = c8.registers[Vx] / 100;
    c8.memory[c8.I + 1] = (c8.registers[Vx] / 10) % 10;
    c8.memory[c8.I + 2] = c8.registers[Vx] % 10;
}

// Store registers V0 through Vx in memory starting at location I
void Interpreter::OP_Fx55() {
    uint8_t Vx = getX();
    for (uint8_t i = 0; i <= Vx; ++i) {
        c8.memory[c8.I + i] = c8.registers[i];
    }
}

// Read registers V0 through Vx from memory starting at location I
void Interpreter::OP_Fx65() {
    uint8_t Vx = getX();
    for (uint8_t i = 0; i <= Vx; ++i) {
        c8.registers[i] = c8.memory[c8.I + i];
    }
}

