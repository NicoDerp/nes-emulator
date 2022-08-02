#include "cpu6502.h"
#include "cpubus.h"
#include "helper.h"

cpu6502::cpu6502()
{
    /** Very ugly lookup table
     *
     * Copied from (https://github.com/OneLoneCoder/olcNES)
     *
     * Contains every variation of all 56 operations.
     * For a total of 256 operations.
     *
    ***/

    // TODO: Implement some unoficcial opcodes
    // NOP, LAX, SAX, DCP, RLA

    // (https://www.nesdev.org/wiki/CPU_unofficial_opcodes)
    // (https://www.nesdev.org/wiki/Programming_with_unofficial_opcodes)

    using a = cpu6502;
	lookup =
	{
     // "BRK", &a::BRK, &a::IMP, 7
        { "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{"*SLO", &a::SLO, &a::IZX, 8 },{"*NOP", &a::NOP, &a::ZP0, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{"*SLO", &a::SLO, &a::ZP0, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::ACC, 2 },{ "???", &a::XXX, &a::IMP, 2 },{"*NOP", &a::NOP, &a::ABS, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{"*SLO", &a::SLO, &a::ABS, 6 },
        { "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{"*NOP", &a::NOP, &a::IMP, 2 },{"*SLO", &a::SLO, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{"*SLO", &a::SLO, &a::ZPX, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{"*NOP", &a::NOP, &a::IMP, 2 },{"*SLO", &a::SLO, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{"*SLO", &a::SLO, &a::ABX, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{"*RLA", &a::RLA, &a::IZX, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{"*RLA", &a::RLA, &a::ZP0, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::ACC, 2 },{ "???", &a::XXX, &a::IMM, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{"*RLA", &a::RLA, &a::ABS, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*RLA", &a::RLA, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{"*RLA", &a::RLA, &a::ZPX, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{"*NOP", &a::NOP, &a::IMP, 2 },{"*RLA", &a::RLA, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{"*RLA", &a::RLA, &a::ABX, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{"*SRE", &a::SRE, &a::IZX, 8 },{"*NOP", &a::NOP, &a::ZP0, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{"*SRE", &a::SRE, &a::ZP0, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::ACC, 2 },{"*ALR", &a::ALR, &a::IMM, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{"*SRE", &a::SRE, &a::ABS, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*SRE", &a::SRE, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{"*SRE", &a::SRE, &a::ZPX, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{"*NOP", &a::NOP, &a::IMP, 2 },{"*SRE", &a::SRE, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{"*SRE", &a::SRE, &a::ABX, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{"*RRA", &a::RRA, &a::IZX, 8 },{"*NOP", &a::NOP, &a::ZP0, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{"*RRA", &a::RRA, &a::ZP0, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::ACC, 2 },{"*ARR", &a::ARR, &a::IMM, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{"*RRA", &a::RRA, &a::ABS, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*RRA", &a::RRA, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{"*RRA", &a::RRA, &a::ZPX, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{"*NOP", &a::NOP, &a::IMP, 2 },{"*RRA", &a::RRA, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{"*RRA", &a::RRA, &a::ABX, 7 },
		{"*NOP", &a::NOP, &a::IMM, 2 },{ "STA", &a::STA, &a::IZX, 6 },{"*NOP", &a::NOP, &a::IMM, 2 },{"*SAX", &a::SAX, &a::IZX, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{"*SAX", &a::SAX, &a::ZP0, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{"*NOP", &a::NOP, &a::IMM, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMM, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{"*SAX", &a::SAX, &a::ABS, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IZY, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{"*SAX", &a::SAX, &a::ZPY, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::ABY, 5 },{ "???", &a::XXX, &a::ABX, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::ABY, 5 },{ "???", &a::XXX, &a::ABY, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{"*LAX", &a::LAX, &a::IZX, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{"*LAX", &a::LAX, &a::ZP0, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{"*LAX", &a::LAX, &a::IMM, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{"*LAX", &a::LAX, &a::ABS, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*LAX", &a::LAX, &a::IZY, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{"*LAX", &a::LAX, &a::ZPY, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{"*LAS", &a::LAS, &a::ABY, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{"*LAX", &a::LAX, &a::ABY, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{"*NOP", &a::NOP, &a::IMM, 2 },{"*DCP", &a::DCP, &a::IZX, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{"*DCP", &a::DCP, &a::ZP0, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{"*AXS", &a::AXS, &a::IMM, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{"*DCP", &a::DCP, &a::ABS, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*DCP", &a::DCP, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{"*DCP", &a::DCP, &a::ZPX, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{"*DCP", &a::DCP, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{"*DCP", &a::DCP, &a::ABX, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{"*NOP", &a::NOP, &a::IMM, 2 },{"*ISC", &a::ISC, &a::IZX, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{"*ISC", &a::ISC, &a::ZP0, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{"*SBC", &a::SBC, &a::IMM, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{"*ISC", &a::ISC, &a::ABS, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{"*ISC", &a::ISC, &a::IZY, 8 },{"*NOP", &a::NOP, &a::ZPX, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{"*ISC", &a::ISC, &a::ZPX, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{"*ISC", &a::ISC, &a::ABY, 7 },{"*NOP", &a::NOP, &a::ABX, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{"*ISC", &a::OSC, &a::ABX, 7 },
	};
}

cpu6502::~cpu6502()
{

}

uint8_t cpu6502::read(uint16_t addr, bool rdonly)
{
    return bus->read(addr, rdonly);
}

void cpu6502::write(uint16_t addr, uint8_t data)
{
    bus->write(addr, data);
}

std::map<uint16_t, std::string> cpu6502::disassemble(uint16_t start, uint16_t end)
{
    std::map<uint16_t, std::string> map;

    uint32_t addr = start;
    uint16_t line;
    uint8_t low, high;
    while (addr<=(uint32_t)end)
    {
        line = addr;
        opcode = read(addr++,true);

        std::string s = "$"+hex(addr-1,4)+"    "+lookup[opcode].name+" ";

        if (lookup[opcode].addrmode == &cpu6502::IMP)
        {
            s += " {IMP}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ACC)
        {
            s += "A  {ACC}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IMM)
        {
            s += "#$"+hex(read(addr++,true),2)+"  {IMM}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::REL)
        {
            uint8_t offset = read(addr++,true);
            s += "$"+hex(offset,2)+" [$"+hex(addr+(int8_t)offset,4)+"]  {REL}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IND)
        {
            low = read(addr++,true);
            high = read(addr++,true);
            s += "$("+hex((high<<8)|low,4)+")  {IND}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABS)
        {
            low = read(addr++,true);
            high = read(addr++,true);
            s += "$"+hex((high<<8)|low,4)+"  {ABS}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABX)
        {
            low = read(addr++,true);
            high = read(addr++,true);
            s += "$"+hex((high<<8)|low,4)+",X  {ABX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ABY)
        {
            low = read(addr++,true);
            high = read(addr++,true);
            s += "$"+hex((high<<8)|low,4)+",Y  {ABY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZP0)
        {
            s += "$"+hex(read(addr++,true),2)+"  {ZP0}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPX)
        {
            s += "$"+hex(read(addr++,true),2)+",X  {ZPX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::ZPY)
        {
            s += "$"+hex(read(addr++,true),2)+",Y  {ZPY}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZX)
        {
            s += "($"+hex(read(addr++,true),2)+",X)  {IZX}";
        }
        else if (lookup[opcode].addrmode == &cpu6502::IZY)
        {
            s += "($"+hex(read(addr++,true),2)+",Y)  {IZY}";
        }
        else
        {
            s += " {XXX}";
        }

        map[line] = s;
    }
    return map;
}

#ifdef LOGMODE
std::string cpu6502::disassembleLine(uint16_t line)
{
    uint8_t low;
    uint8_t high;
    std::string tmp = "";

    uint16_t addr = line;

    opcode = read(addr++,true);

    // lookup[opcode].name
    std::string f = hex(addr-1,4);

    std::string b = hex(opcode,2);

    std::string c = lookup[opcode].name;

    // Operand name
    uint8_t len = 3;

    if (lookup[opcode].addrmode == &cpu6502::IMP)
    {

    }
    else if (lookup[opcode].addrmode == &cpu6502::ACC)
    {

    }
    else if (lookup[opcode].addrmode == &cpu6502::IMM)
    {
        tmp = hex(read(addr++, true),2);
        b += " " + tmp;
        c += " #$" + tmp;
    }
    else if (lookup[opcode].addrmode == &cpu6502::REL)
    {
        uint8_t offset = read(addr++,true);
        b += " " + hex(offset,2);
        c += " $"+hex(addr+(int8_t)offset,4);
    }
    else if (lookup[opcode].addrmode == &cpu6502::IND)
    {
        low = read(addr++,true);
        high = read(addr++,true);
        b += " " + hex(low,2) + " " + hex(high,2);
        c += " $("+hex((high<<8)|low,4)+")";
    }
    else if (lookup[opcode].addrmode == &cpu6502::ABS)
    {
        low = read(addr++,true);
        high = read(addr++,true);
        b += " " + hex(low,2) + " " + hex(high,2);
        c += " $"+hex((high<<8)|low,4);
    }
    else if (lookup[opcode].addrmode == &cpu6502::ABX)
    {
        low = read(addr++,true);
        high = read(addr++,true);
        b += " " + hex(low,2) + " " + hex(high,2);
        c += " $"+hex((high<<8)|low,4)+",X";
    }
    else if (lookup[opcode].addrmode == &cpu6502::ABY)
    {
        low = read(addr++,true);
        high = read(addr++,true);
        b += " " + hex(low,2) + " " + hex(high,2);
        c += " $"+hex((high<<8)|low,4)+",Y";
    }
    else if (lookup[opcode].addrmode == &cpu6502::ZP0)
    {
        tmp = hex(read(addr++,true),2);
        b += " " + tmp;
        c += " $" + tmp;
    }
    else if (lookup[opcode].addrmode == &cpu6502::ZPX)
    {
        tmp = hex(read(addr++,true),2);
        b += " " + tmp;
        c += " $"+tmp+",X";
    }
    else if (lookup[opcode].addrmode == &cpu6502::ZPY)
    {
        tmp = hex(read(addr++,true),2);
        b += " " + tmp;
        c += " $"+tmp+",Y";
    }
    else if (lookup[opcode].addrmode == &cpu6502::IZX)
    {
        tmp = hex(read(addr++,true),2);
        b += " " + tmp;
        c += " ($"+tmp+",X)";
    }
    else if (lookup[opcode].addrmode == &cpu6502::IZY)
    {
        tmp = hex(read(addr++,true),2);
        b += " " + tmp;
        c += " ($"+tmp+",Y)";
    }
    else
    {
        // XXX
    }

    std::string s = f + "  " + b;

    for (int i=b.length();i<10;i++)
    {
        s += " ";
    }

    s += c;

    for (int i=c.length();i<32;i++)
    {
        s += " ";
    }

    s += "A:"+hex(a,2)+" X:"+hex(x,2)+" Y:"+hex(y,2)+" P:"+hex(status,2)+" SP:"+hex(stackptr,2);
    s += "\n";

    return s;
}

#endif


bool cpu6502::getFlag(FLAGS6502 flag)
{
    return (bool)(status & flag);
}

// flag:
// 1, 2, 4, 8, 16, 32, 64, 128

// status:
// [ 1 0 0 0 0 1 0 1 ]
//       ^
//       |
//       0
//    32
// [ 1 1 0 1 1 1 1 1 ]

void cpu6502::setFlag(FLAGS6502 flag, bool value)
{
    if (value)
        status |= flag;
    else
        status &= ~flag;
}

void cpu6502::push(uint8_t val)
{
    write(stackptr+0x0100, val);
    stackptr--;
}

uint8_t cpu6502::pop()
{
    // Stack underflow
    //if (stackptr > 0xFD)
    //{
    //    printf("[WARNING] Stack underflow!");
    //    // exit(0) ??
    //}

    stackptr++;
    return read(stackptr+0x0100);
}

void cpu6502::clock()
{
    // No more cycles left for operation so fetch next one
    if (cycles == 0)
    {

#ifdef LOGMODE

        if (logfile == nullptr)
        {
            logfile = fopen("nestest.log", "wt");
        }

        if (logfile != nullptr)
        {
            std::string s = disassembleLine(pc);
            fprintf(logfile, s.c_str());
        }

#endif

        // Read opcode and increment program counter
        opcode = read(pc);
        pc++;

        cycles = lookup[opcode].cycles;

        //printf("Emulating '%s' [%d]\n", lookup[opcode].name.c_str(), opcode);
        //printf("Cycles %d\n", cycles);

        setFlag(U, 1);

        bool c1 = (this->*lookup[opcode].addrmode)();

        bool c2 = (this->*lookup[opcode].operation)();

        if (c1&&c2)
            cycles++;

        setFlag(U, 1);

    }
    // One cycle has passed
    cycles--;
    clockCount++;
}

void cpu6502::reset()
{
    a = 0;
    x = 0;
    y = 0;

    stackptr = 0xFD;

    // Everything is zero exept for the break, unused bit and interrupt disable
    // nestest differs so change back
    // status = 0x00 | B | U | I;
    status = 0x00 | U | I;

    printf("STAUS: %s\n", hex(status,2).c_str());

    // Read where the program starts
    uint8_t low = read(0xFFFC);
    uint8_t high = read(0xFFFD);

    pc = (high << 8) | low;
    printf("PC: 0x%s\n", hex(pc,4).c_str());

    addr_rel = 0x0000;
    addr_abs = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void cpu6502::nmi()
{
    push(pc>>8); // Push high byte
    push(pc&0x00FF); // Push low byte

    setFlag(B, 0);
    setFlag(U, 1);

    push(status); // Push status register

    setFlag(I, 1);

    // Read new program counter
    uint8_t low = read(0xFFFA);
    uint8_t high = read(0xFFFB);

    pc = (high << 8) | low;

    cycles = 8;

}

void cpu6502::irq()
{
    if (!getFlag(I))
    {
        push(pc>>8); // Push high byte
        push(pc&0x00FF); // Push low byte

        setFlag(B, 0);
        setFlag(U, 1);

        push(status); // Push status register

        setFlag(I, 1);

        // Read new program counter
        uint8_t low = read(0xFFFE);
        uint8_t high = read(0xFFFF);

        pc = (high << 8) | low;

        cycles = 7;
    }
}

uint8_t cpu6502::fetch()
{
    if (lookup[opcode].addrmode != &cpu6502::IMP &&
        lookup[opcode].addrmode != &cpu6502::ACC)
    {
        fetched = read(addr_abs);
    }

    return fetched;
}

uint8_t cpu6502::IMP()
{
    return 0;
}

uint8_t cpu6502::ACC()
{
    fetched = a;

    return 0;
}

uint8_t cpu6502::IMM()
{
    addr_abs = pc;
    pc++;

    return 0;
}

uint8_t cpu6502::REL()
{
    addr_rel = read(pc);
    pc++;
    if (addr_rel & 0x80) // If it is signed
        addr_rel |= 0xFF00; // ???

    return 0;
}

uint8_t cpu6502::IND()
{
    uint16_t ptr_low = read(pc);
    pc++;
    uint16_t ptr_high = read(pc);
    pc++;

    uint16_t ptr = (ptr_high << 8) | ptr_low;

    // To simulate a bug in the nes.
    // (https://www.nesdev.org/6502bugs.txt)
    if (ptr_low == 0x00FF)
    {
        // Simulate bug by clearing low bytes in high bytes
        addr_abs = (read(ptr & 0xFF00) << 8) | (read(ptr));
    }
    else
    {
        // Normally
        addr_abs = (read(ptr+1) << 8) | (read(ptr));
    }

    return 0;
}

uint8_t cpu6502::ABS()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;

    addr_abs = (high << 8) | low;

    return 0;
}

uint8_t cpu6502::ABX()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;

    addr_abs = (high << 8) | low;
    addr_abs += x;

    // Page has overflowed, requiring an extra cycle
    if ((addr_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;
}

uint8_t cpu6502::ABY()
{
    uint16_t low = read(pc);
    pc++;
    uint16_t high = read(pc);
    pc++;

    addr_abs = (high << 8) | low;
    addr_abs += y;

    // Page has overflowed, requiring an extra cycle
    if ((addr_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;

}

uint8_t cpu6502::ZP0()
{
    addr_abs = read(pc);
    pc++;

    //addr_abs &= 0x00FF; //??? unnecessary

    return 0;
}

uint8_t cpu6502::ZPX()
{
    addr_abs = read(pc);
    pc++;

    addr_abs += x;
    addr_abs &= 0x00FF; // So it wraps around incase of overflow

    return 0;
}

uint8_t cpu6502::ZPY()
{
    addr_abs = read(pc);
    pc++;

    addr_abs += y;
    addr_abs &= 0x00FF; // So it wraps around incase of overflow

    return 0;
}

uint8_t cpu6502::IZX()
{
    uint16_t offset = read(pc);
    pc++;

    uint16_t low = read((uint16_t)(offset+(uint16_t)x)&0x00FF);
    uint16_t high = read((uint16_t)(offset+(uint16_t)x+1)&0x00FF);
    addr_abs = (high << 8) | low;

    return 0;
}

uint8_t cpu6502::IZY()
{
    uint16_t offset = read(pc);
    pc++;

    uint16_t low = read(offset);
    uint16_t high = read((offset+1)&0x00FF);

    addr_abs = (high << 8) | low;
    addr_abs += y;

    // Different high bytes after addition means overflow
    // Therefore an extra cycle is needed
    if ((addr_abs & 0xFF00) != (high << 8))
        return 1;
    return 0;
}


// Operations

uint8_t cpu6502::ADC()
{
    fetch();
    if (getFlag(D))
    {
        uint8_t one = (a&0x0F)+(fetched&0x0F)+getFlag(C);
        uint8_t ten = (a>>4)+(fetched>>4);
        if (one>9)
        {
            ten++;
            one-=10;
        }
        // buruuh wtf idkam
        setFlag(V, ten&0x08);
        setFlag(C, ten>9);
        if (ten>9)
            ten-=10;
        a = (ten*16)+one;
    }
    else
    {
        uint16_t tmp = a + fetched + getFlag(C);
        setFlag(V, (a&0x80==fetched&0x80)&(a&0x80!=tmp&0x80));
        setFlag(Z, a==0);
        setFlag(C, tmp>0xFF);
        setFlag(N, a&0x80);
        a = tmp&0x00FF;
    }
    return 1;
}

uint8_t cpu6502::AND()
{
    fetch();
    a &= fetched;
    setFlag(Z, a==0x00); // Set the zero flag
    setFlag(N, a&0x80); // Set the negative flag
    return 1;
}

uint8_t cpu6502::ASL()
{
    fetch();
    uint8_t tmp = fetched << 1;
    setFlag(Z, tmp==0x00); // Set the zero flag
    setFlag(C, fetched&0x80); // Set carry to leftmost bit
    setFlag(N, tmp&0x80); // Set the negative bit

    if (lookup[opcode].addrmode == &cpu6502::ACC)
        a = tmp;
    else
        write(addr_abs, tmp);

    return 0;
}

uint8_t cpu6502::BCC()
{
    if (!getFlag(C))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BCC jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BCS()
{
    if (getFlag(C))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BCS jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;

}

uint8_t cpu6502::BEQ()
{
    if (getFlag(Z))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BEC jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;

}

uint8_t cpu6502::BIT()
{
    fetch();
    uint8_t tmp = a & fetched;
    //    setFlag(V, fetched&(1<<6)); // Set the overflow flag
    setFlag(V, (a&0x80==fetched&0x80)&(a&0x80!=tmp&0x80));
    setFlag(Z, tmp==0); // Set the zero flag
    setFlag(N, fetched&(1<<7)); // Set the negative flag
    return 0;

}

uint8_t cpu6502::BMI()
{
    if (getFlag(N))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BMI jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;

}

uint8_t cpu6502::BNE()
{
    if (!getFlag(Z))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BNE jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BPL()
{
    if (!getFlag(N))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BPL jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BRK()
{
    //pc++;

    push((pc>>8)&0x00FF); // High byte
    push(pc&0x00FF); // Low byte

    setFlag(B, 1);
    push(status);
    setFlag(B, 0);
    setFlag(I, 1);

    uint8_t low = read(0xFFFE);
    uint8_t high = read(0xFFFF);
    pc = (high << 8) | low;
    return 0;
}

uint8_t cpu6502::BVC()
{
    if (!getFlag(V))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BVC jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::BVS()
{
    if (getFlag(V))
    {
        cycles++;
        addr_abs = pc + addr_rel;
        //printf("BCS jumped from 0x%s to 0x%s\n", hex(pc,4).c_str(), hex(addr_abs,4).c_str());
        if ((addr_abs & 0xFF00) != (pc & 0xFF00))
            cycles++;
        pc = addr_abs;
    }
    return 0;
}

uint8_t cpu6502::CLC()
{
    setFlag(C, 0);
    return 0;
}

uint8_t cpu6502::CLD()
{
    setFlag(D, 0);
    return 0;
}

uint8_t cpu6502::CLI()
{
    setFlag(I, 0);
    return 0;
}

uint8_t cpu6502::CLV()
{
    setFlag(V, 0);
    return 0;
}

uint8_t cpu6502::CMP()
{
    fetch();

    setFlag(Z, a==fetched);
    setFlag(N, (a-fetched)&0x80);
    setFlag(C, fetched <= a);
    return 1;
}

uint8_t cpu6502::CPX()
{
    fetch();

    setFlag(C, x >= fetched);
    setFlag(N, (x-fetched)&0x80);
    setFlag(Z, x==fetched);
    return 0;
}

uint8_t cpu6502::CPY()
{
    fetch();

    setFlag(C, y >= fetched);
    setFlag(N, (y-fetched)&0x80);
    setFlag(Z, y==fetched);
    return 0;
}

uint8_t cpu6502::DEC()
{
    fetch();
    setFlag(N, (fetched-1)&0x80);
    setFlag(Z, (fetched-1)==0);
    write(addr_abs, fetched-1);
    return 0;
}

uint8_t cpu6502::DEX()
{
    x -= 1;
    setFlag(N, x&0x80);
    setFlag(Z, x==0);
    return 0;
}

uint8_t cpu6502::DEY()
{
    y -= 1;
    setFlag(N, y&0x80);
    setFlag(Z, y==0);
    return 0;
}

uint8_t cpu6502::EOR()
{
    fetch();
    a ^= fetched;
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 0;
}

uint8_t cpu6502::INC()
{
    fetch();
    setFlag(N, (fetched+1)&0x80);
    setFlag(Z, (fetched+1)==0);
    write(addr_abs, fetched+1);
    return 0;
}

uint8_t cpu6502::INX()
{
    x += 1;
    setFlag(N, x&0x80);
    setFlag(Z, x==0);
    return 0;
}

uint8_t cpu6502::INY()
{
    y += 1;
    setFlag(N, y&0x80);
    setFlag(Z, y==0);
    return 0;
}

uint8_t cpu6502::JMP()
{
    pc = addr_abs;
    return 0;
}

uint8_t cpu6502::JSR()
{
    push((pc-1)>>8); // High byte
    push((pc-1)&0x00FF); // Low byte
    pc = addr_abs;
    return 0;
}

uint8_t cpu6502::LDA()
{
    a = fetch();
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 1;
}

uint8_t cpu6502::LDX()
{
    x = fetch();
    setFlag(N, x&0x80);
    setFlag(Z, x==0);
    return 1;
}

uint8_t cpu6502::LDY()
{
    y = fetch();
    setFlag(N, y&0x80);
    setFlag(Z, y==0);
    return 1;
}

uint8_t cpu6502::LSR()
{
    fetch();
    uint8_t tmp = fetched >> 1;
    setFlag(N, 0);
    setFlag(Z, tmp==0);
    setFlag(C, fetched&0x01);
    if (lookup[opcode].addrmode == &cpu6502::ACC)
        a = tmp;
    else
        write(addr_abs, tmp);
    return 0;
}

uint8_t cpu6502::NOP()
{
    /** Holy Shit
     *
     * (https://www.nesdev.org/wiki/CPU_unofficial_opcodes)
     *
     * All official and unofficial NOPs.
     *
    ***/

    switch (opcode)
    {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:

    case 0x1A:
    case 0x3A:
    case 0x5A:
    case 0x7A:
    case 0xDA:
    case 0xFA:

    case 0x14:
    case 0x34:
    case 0x54:
    case 0x74:
    case 0xD4:
    case 0xF4:

    case 0x0C:

        //    case 0xEA: The official one

    case 0x89:

    case 0x04:
    case 0x44:
    case 0x64:

    case 0x82:
    case 0xC2:
    case 0xE2:

    case 0x80:
        return 1;
        break;
    }
    return 0;
}

uint8_t cpu6502::ORA()
{
    fetch();
    a |= fetched;
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 1;
}

uint8_t cpu6502::PHA()
{
    push(a);
    return 0;
}

uint8_t cpu6502::PHP()
{
    setFlag(B, 0);
    setFlag(U, 1);
    push(status);
    return 0;
}

uint8_t cpu6502::PLA()
{
    a = pop();
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 0;
}

uint8_t cpu6502::PLP()
{
    status = pop();
    setFlag(U, 1);
    setFlag(B, 0);
    return 0;
}

uint8_t cpu6502::ROL()
{
    fetch();
    uint8_t tmp = fetched << 1;
    tmp |= getFlag(C); // Set LSB to C
    setFlag(N, fetched&0x40);
    setFlag(Z, tmp==0);
    setFlag(C, fetched&0x80);
    if (lookup[opcode].addrmode == &cpu6502::ACC)
        a = tmp;
    else
        write(addr_abs, tmp);
    return 0;
}

uint8_t cpu6502::ROR()
{
    fetch();
    uint8_t tmp = fetched >> 1;
    tmp |= (getFlag(C)<<7); // Set MSB to C
    setFlag(N, getFlag(C));
    setFlag(Z, tmp==0);
    setFlag(C, fetched&0x01);
    if (lookup[opcode].addrmode == &cpu6502::ACC)
        a = tmp;
    else
        write(addr_abs, tmp);
    return 0;
}

uint8_t cpu6502::RTI()
{
    status = pop();
    setFlag(B, 0);
    setFlag(U, 1);

    uint8_t low = pop();
    uint8_t high = pop();
    pc = (high << 8) | low;
    return 0;
}

uint8_t cpu6502::RTS()
{
    uint8_t low = pop();
    uint8_t high = pop();
    pc = (high << 8) | low;
    pc++;
    return 0;
}

uint8_t cpu6502::SBC()
{
    fetch();
    if (getFlag(D))
    {
        uint8_t one;
        uint8_t ten;
        bool borrow = false;
        if (a&0x0F+getFlag(C)<fetched&0x0F)
        {
            borrow = true;
            one = 9+(a&0x0F)-(fetched&0x0F)+getFlag(C);
        }
        else
        {
            one = (a&0x0F)-(fetched&0x0F)-getFlag(C)+1;
        }

        if ((a>>4)<(fetched>>4))
        {
            ten = 10+(a>>4)-(fetched>>4);
            setFlag(C, 1);
        }
        else
        {
            ten = (a>>4)-(fetched>>4);
            setFlag(C, 0);
        }

        if (borrow)
        {
            if (ten==0)
                ten = 9;
            else
                ten--;
        }

        // No N, V and Z update
        a = (ten*16)+one;
    }
    else
    {
        uint16_t tmp = a-fetched + getFlag(C) - 1;
        a = tmp&0x00FF;
        setFlag(V, (a&0x80==fetched&0x80)&(a&0x80!=tmp&0x80));
        setFlag(C, a+getFlag(C)>=fetched);
        setFlag(N, a&0x80);
        setFlag(Z, a==0);
    }
    return 1;
}

uint8_t cpu6502::SEC()
{
    setFlag(C, 1);
    return 0;
}

uint8_t cpu6502::SED()
{
    setFlag(D, 1);
    return 0;
}

uint8_t cpu6502::SEI()
{
    setFlag(I, 1);
    return 0;
}

uint8_t cpu6502::STA()
{
    write(addr_abs, a);
    return 0;
}

uint8_t cpu6502::STX()
{
    write(addr_abs, x);
    return 0;
}

uint8_t cpu6502::STY()
{
    write(addr_abs, y);
    return 0;
}

uint8_t cpu6502::TAX()
{
    x = a;
    setFlag(N, x&0x80);
    setFlag(Z, x==0);
    return 0;
}

uint8_t cpu6502::TAY()
{
    y = a;
    setFlag(N, y&0x80);
    setFlag(Z, y==0);
    return 0;
}

uint8_t cpu6502::TSX()
{
    x = stackptr;
    setFlag(N, x&0x80);
    setFlag(Z, x==0);
    return 0;
}

uint8_t cpu6502::TXA()
{
    a = x;
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 0;
}

uint8_t cpu6502::TXS()
{
    stackptr = x;
    return 0;
}

uint8_t cpu6502::TYA()
{
    a = y;
    setFlag(N, a&0x80);
    setFlag(Z, a==0);
    return 0;
}




uint8_t cpu6502::LAX()
{
    return 0;
}

uint8_t cpu6502::SAX()
{
    return 0;
}

uint8_t cpu6502::DCP()
{
    return 0;
}

uint8_t cpu6502::RLA()
{
    return 0;
}

uint8_t cpu6502::RRA()
{
    return 0;
}

uint8_t cpu6502::SLO()
{
    return 0;
}

uint8_t cpu6502::SRE()
{
    return 0;
}



uint8_t cpu6502::XXX()
{
    return 0;
}

