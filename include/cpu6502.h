//#ifndef INCLUDE_CPU6502_H
//#define INCLUDE_CPU6502_H

#pragma once

#include <stdint.h>
#include <vector>
#include <string>
#include <map>

#define LOGMODE

// Forward declaration
class Bus;

class cpu6502
{
public:
    cpu6502();
    ~cpu6502();

    void connectBus(class CPUBus *n) { bus = n; };

    void clock();
    void reset();
    void nmi();
    void irq();

    bool complete() { return cycles == 0; }
    std::map<uint16_t, std::string> disassemble(uint16_t start, uint16_t end);\

#ifdef LOGMODE
    std::string disassembleLine(uint16_t line);
#endif

public:
    enum FLAGS6502
    {
        C = (1 << 0), // Carry flag
        Z = (1 << 1), // Zero flag
	    I = (1 << 2), // Interrupt disable flag
        D = (1 << 3), // Decimal flag
        B = (1 << 4), // Break flag
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow flag
        N = (1 << 7)  // Negative flag
    };

    bool getFlag(FLAGS6502 flag);
    void setFlag(FLAGS6502 flag, bool value);

    uint8_t a = 0x00; // Accumulator register
    uint8_t x = 0x00; // X register
    uint8_t y = 0x00; // Y register

    uint16_t pc = 0x0000; // Program counter
    uint8_t stackptr = 0x00; // Stack pointer
    uint8_t status = 0x00; // Status register (the flags)

    uint8_t cycles = 0x00; // Cycles left for operation
    uint8_t opcode = 0x00; // Current opcode

    uint32_t clockCount = 0;

    uint8_t fetched = 0x00; // The fetched data
    uint16_t addr_abs = 0x0000; // Address computed by the addres mode for operand to use
    uint16_t addr_rel = 0x0000; // Address computed by the addres mode for operand to use

private:
	CPUBus *bus = nullptr;

	uint8_t read(uint16_t addr, bool rdonly=false);
	void write(uint16_t addr, uint8_t data);

    void push(uint8_t val);
    uint8_t pop();

    uint8_t fetch();

    struct INSTRUCTION
    {
        std::string name;
        uint8_t(cpu6502::*operation)(void) = nullptr;
        uint8_t(cpu6502::*addrmode)(void) = nullptr;
        uint8_t cycles = 0;
    };

    std::vector<INSTRUCTION> lookup;

    /** Addressing modes
     *
     * (https://www.nesdev.org/wiki/CPU_addressing_modes)
     *
     * Describes how the operation will fetch some value.
     * Some operations need no values.
     *
    ***/

    uint8_t IMP(); // Implicit
    uint8_t ACC(); // Accumulator
    uint8_t IMM(); // Immediate
    uint8_t REL(); // Relative
    uint8_t IND(); // Indirect

    uint8_t ABS(); // Absolute
    uint8_t ABX(); // Absolute indexed offset x
    uint8_t ABY(); // Absolute indexed offset y

    uint8_t ZP0(); // Zero page
    uint8_t ZPX(); // Zero page offset x
    uint8_t ZPY(); // Zero page offset y

    uint8_t IZX(); // Indexed indirect offset x
    uint8_t IZY(); // Indexed indirect offset y


    /** Opcodes
     *
     * (https://www.nesdev.org/wiki/CPU_unofficial_opcodes)
     * (http://www.6502.org/tutorials/6502opcodes.html)
     *
     * All of the opcodes for the 6502 except some of the unofficial ones.
     * Those are captured in XXX.
     *
    ***/

    uint8_t ADC(); 	uint8_t AND(); 	uint8_t ASL(); 	uint8_t BCC();
 	uint8_t BCS(); 	uint8_t BEQ(); 	uint8_t BIT(); 	uint8_t BMI();
 	uint8_t BNE(); 	uint8_t BPL(); 	uint8_t BRK(); 	uint8_t BVC();
 	uint8_t BVS(); 	uint8_t CLC();  uint8_t CLD(); 	uint8_t CLI();
 	uint8_t CLV(); 	uint8_t CMP(); 	uint8_t CPX(); 	uint8_t CPY();
 	uint8_t DEC(); 	uint8_t DEX(); 	uint8_t DEY(); 	uint8_t EOR();
 	uint8_t INC(); 	uint8_t INX(); 	uint8_t INY(); 	uint8_t JMP();
    uint8_t JSR(); 	uint8_t LDA(); 	uint8_t LDX(); 	uint8_t LDY();
 	uint8_t LSR(); 	uint8_t NOP(); 	uint8_t ORA(); 	uint8_t PHA();
 	uint8_t PHP(); 	uint8_t PLA(); 	uint8_t PLP(); 	uint8_t ROL();
 	uint8_t ROR(); 	uint8_t RTI();  uint8_t RTS(); 	uint8_t SBC();
 	uint8_t SEC(); 	uint8_t SED(); 	uint8_t SEI(); 	uint8_t STA();
 	uint8_t STX(); 	uint8_t STY(); 	uint8_t TAX(); 	uint8_t TAY();
 	uint8_t TSX(); 	uint8_t TXA(); 	uint8_t TXS(); 	uint8_t TYA();

    uint8_t LAX();  uint8_t SAX();  uint8_t DCP();  uint8_t RLA();
    uint8_t RRA();  uint8_t SLO();  uint8_t SRE();  uint8_t ALR();
    uint8_t ARR();  uint8_t LAS();  uint8_t AXS();  uint8_t ISC();

    uint8_t XXX();

#ifdef LOGMODE
private:
    FILE* logfile = nullptr;
#endif

};

//#endif /* INCLUDE_CPU6502_H */
