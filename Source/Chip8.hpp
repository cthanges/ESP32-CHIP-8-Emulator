#pragma once
#include <cstdint>
#include <random>

// CHIP-8 Specifications
const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class Chip8
{
public:
	Chip8();
	void LoadROM(char const* filename);
	void Cycle();

	uint8_t GetSoundTimer() const { return soundTimer; } // Expose sound timer for audio playback

	uint8_t keypad[KEY_COUNT]{};
	uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};

private:
	// Secondary Tables
	void Table0();
	void Table8();
	void TableE();
	void TableF();

	// CHIP-8 Instructions
	void OP_NULL(); // No operation
	void OP_00E0(); // CLS (clear the display)
	void OP_00EE(); // RET (return from a subroutine)
	void OP_1NNN(); // JP addr (jump to address NNN)
	void OP_2NNN(); // CALL addr (call subroutine at address NNN)
	void OP_3XKK(); // SE Vx, byte (skip next instruction if Vx == kk)
	void OP_4XKK(); // SNE Vx, byte (skip next instruction if Vx != kk)
	void OP_5XY0(); // SE Vx, Vy (skip next instruction if Vx == Vy)
	void OP_6XKK(); // LD Vx, byte (set Vx = KK)
	void OP_7XKK(); // ADD Vx, byte (set Vx = Vx + KK)
	void OP_8XY0(); // LD Vx, Vy (set Vx = Vy)
	void OP_8XY1(); // OR Vx, Vy (set Vx = Vx | Vy)
	void OP_8XY2(); // AND Vx, Vy (set Vx = Vx & Vy)
	void OP_8XY3(); // XOR Vx, Vy (set Vx = Vx ^ Vy)
	void OP_8XY4(); // ADD Vx, Vy (set Vx = Vx + Vy, set VF = carry) 
	void OP_8XY5(); // SUB Vx, Vy (set Vx = Vx - Vy, set VF = NOT borrow)
	void OP_8XY6(); // SHR Vx (set Vx = Vx >> 1, set VF = LSB before shift)
	void OP_8XY7(); // SUBN Vx, Vy (set Vx = Vy - Vx, set VF = NOT borrow)
	void OP_8XYE(); // SHL Vx (set Vx = Vx << 1, set VF = MSB before shift)
	void OP_9XY0(); // SNE Vx, Vy (skip next instruction if Vx != Vy)
	void OP_ANNN(); // LD I, addr (set I = NNN)
	void OP_BNNN(); // JP V0, addr (jump to location V0 + NNN)
	void OP_CXKK(); // RND Vx, byte (set Vx = random byte & KK)
	void OP_DXYN(); // DRW Vx, Vy, nibble (display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision)
	void OP_EX9E(); // SKP Vx (skip next instruction if key with Vx value is pressed)
	void OP_EXA1(); // SKNP Vx (skip next instruction if key with Vx value is not pressed)
	void OP_FX07(); // LD Vx, DT (set Vx = delay timer value)
	void OP_FX0A(); // LD Vx, K (wait for a key press and store key value in Vx)
	void OP_FX15(); // LD DT, Vx (set delay timer = Vx)
	void OP_FX18(); // LD ST, Vx (set sound timer = Vx)
	void OP_FX1E(); // ADD I, Vx (set I = I + Vx)
	void OP_FX29(); // LD F, Vx (set I = location of sprite for digit Vx)
	void OP_FX33(); // LD B, Vx (store BCD representation of Vx in memory locations I, I+1, and I+2)
	void OP_FX55(); // LD [I], Vx (store registers V0 through Vx in memory starting at location I)
	void OP_FX65(); // LD Vx, [I] (read registers V0 through Vx from memory starting at location I)

	uint8_t memory[MEMORY_SIZE]{};
	uint8_t registers[REGISTER_COUNT]{};
	uint16_t index{};
	uint16_t pc{};
	uint8_t delayTimer{};
	uint8_t soundTimer{};
	uint16_t stack[STACK_LEVELS]{};
	uint8_t sp{};
	uint16_t opcode{};

	std::default_random_engine randGen;
	std::uniform_int_distribution<uint8_t> randByte;

	typedef void (Chip8::*Chip8Func)();
	Chip8Func table[0xF + 1];
	Chip8Func table0[0xE + 1];
	Chip8Func table8[0xE + 1];
	Chip8Func tableE[0xE + 1];
	Chip8Func tableF[0x65 + 1];
};