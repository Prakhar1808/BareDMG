#include <core/utils.h>
#include <gbemu.h>
#include <stdio.h>

/*
Memory Map:
https://gbdev.io/pandocs/Memory_Map.html#memory-map

0x0000 - 0x00FF : Boot ROM (disabled after boot)
0x0100 - 0x3FFF : ROM Bank 0 (fixed, from cartridge)
0x4000 - 0x7FFF : ROM Bank N (switchable via MBC)

0x8000 - 0x9FFF : Video RAM (VRAM) - 8 KB
0xA000 - 0xBFFF : External RAM (on cartridge, if present)
0xC000 - 0xCFFF : Work RAM Bank 0 (WRAM) - 4 KB
0xD000 - 0xDFFF : Work RAM Bank 1 (WRAM) - 4 KB (in CGB mode, switchange bank 1-7)
0xE000 - 0xFDFF : Echo RAM (mirror of 0xC000-0xDDFF)
0xFE00 - 0xFE9F : Object Attribute Memory (OAM) - Sprite data
0xFEA0 - 0xFEFF : Unusable (reads as 0x00)

0xFF00 - 0xFF7F : I/O Registers (hardware control)
0xFF80 - 0xFFFE : High RAM (HRAM) - 127 bytes
0xFFFF          : Interrupt Enable Register (IE)
*/

// Read one byte from memory
u8 mmu_read(GameBoy *gb, u16 addr) {
    // ---------------------------
    // ROM Bank 0 (0x0000 - 0x3FFF) - Fixed
    // ---------------------------
    if (addr < 0x4000) {
        if (addr < gb->cart.rom_size)
            return gb->cart.rom[addr];
        return 0xFF; // Open bus
    }

    // ---------------------------
    // ROM Bank N (0x4000 - 0x7FFF) - Switchable
    // ---------------------------
    if (addr < 0x8000) {
        // For now, just read from ROM directly
        // TODO: MBC will handle bank switching
        if (addr < gb->cart.rom_size)
            return gb->cart.rom[addr];
        return 0xFF;
    }

    // ---------------------------
    // VRAM (0x8000 - 0x9FFF) - 8 KB
    // ---------------------------
    if (addr < 0xA000) {
        // TODO: Implement when the PPU is ready
        // For now, just return from VRAM array
        return gb->vram[addr - 0x8000];
    }

    // ---------------------------
    // External RAM (0xA000 - 0xBFFF) - Cartridge RAM
    // ---------------------------
    if (addr < 0xC000) {
        // TODO: Implement with MBC (bank switching, enalbe/disable)
        // For now, direct access if RAM exists
        u16 ram_addr = addr - 0xA000;
        if (ram_addr < gb->cart.ram_size)
            return gb->cart.ram[ram_addr];
        return 0xFF;
    }

    // ---------------------------
    // Work RAM (0xC000 - 0xDFFF) - Cartridge RAM
    // ---------------------------
    if (addr < 0xE000) {
        return gb->wram[addr - 0xC000];
    }

    // ---------------------------
    // Echo RAM (0xE000 - 0xFDFF) - Mirror of WRAM
    // ---------------------------
    if (addr < 0xFE00) {
        return gb->wram[addr - 0xE000];
    }

    // ---------------------------
    // OAM (0xFE00 - 0xFE9F) - Sprite Attribute Table
    // ---------------------------
    if (addr < 0xFEA0) {
        // TODO: Implement when PPU is ready
        return gb->oam[addr - 0xFE00];
    }

    // ---------------------------
    // Un-usable (0xFEA0 - 0xFEFF)
    // ---------------------------
    if (addr < 0xFF00) {
        // Writes ignored
        return 0x00;
    }

    // ---------------------------
    // I/O Registers 0xFF00 - 0xFF7F
    // ---------------------------
    if (addr < 0xFF80) {
        return io_read(gb, addr);
    }

    // ---------------------------
    // HRAM 0xFF80 - 0xFFFE 127 bytes
    // ---------------------------
    if (addr < 0xFFFF) {
        return gb->hram[addr - 0xFF80];
    }

    // ---------------------------
    // Interrupt Enable (0xFFFF)
    // ---------------------------
    if (addr == 0xFFFF) {
        return gb->ie_register;
    }

    // Ideally, control must never reach here
    return 0xFF; // Open Bus
}

// Write one Byte to memory
void mmu_write(GameBoy *gb, u16 addr, u8 value) {
    // ---------------------------
    // ROM (0x0000 - 0x7FFF) - MBC Control
    // ---------------------------
    if (addr < 0x8000) {
        // TODO: Implement when MBC is ready
        // Writes to ROM control MBC (bank switching, RAM enable, etc)
        // Ignore writes to ROM for now
        return;
    }

    // ---------------------------
    // VRAM (0x8000 - 0x9FFF) - 8 KB
    // ---------------------------
    if (addr < 0xA000) {
        // TODO: Check if VRAM is accessible (not during PPU mode 3)
        gb->vram[addr - 0x8000] = value;
        return;
    }

    // ---------------------------
    // External RAM (0xA000 - 0xBFFF) - Cartridge RAM
    // ---------------------------
    if (addr < 0xC000) {
        // TODO: Implement with MBC (check if RAM is enabled)
        u16 ram_addr = addr - 0xA000;
        if (ram_addr < gb->cart.ram_size) {
            gb->cart.ram[ram_addr] = value;
        }
        return;
    }

    // ---------------------------
    // Work RAM (0xC000 - 0xDFFF) - Cartridge RAM
    // ---------------------------
    if (addr < 0xE000) {
        gb->wram[addr - 0xC000] = value;
        return;
    }

    // ---------------------------
    // Echo RAM (0xE000 - 0xFDFF) - Mirror of WRAM
    // ---------------------------
    if (addr < 0xFE00) {
        // Write to WRAM (mirrored)
        gb->wram[addr - 0xE000] = value;
        return;
    }

    // ---------------------------
    // OAM (0xFE00 - 0xFE9F) - Sprite Attribute Table
    // ---------------------------
    if (addr < 0xFEA0) {
        // TODO: Check if OAM is accessible (not during PPU mode 2/3)
        gb->oam[addr - 0xFE00] = value;
        return;
    }

    // ---------------------------
    // Un-usable (0xFEA0 - 0xFEFF)
    // ---------------------------
    if (addr < 0xFF00) {
        // Writes ignored
        return;
    }

    // ---------------------------
    // I/O Registers (0xFF00 - 0xFF7F)
    // ---------------------------
    if (addr < 0xFF80) {
        io_write(gb, addr, value);
        return;
    }

    // ---------------------------
    // HRAM 0xFF80 - 0xFFFE 127 bytes
    // ---------------------------
    if (addr < 0xFFFF) {
        gb->hram[addr - 0xFF80] = value;
        return;
    }

    // ---------------------------
    // Interrupt Enable (0xFFFF)
    // ---------------------------
    if (addr == 0xFFFF) {
        gb->ie_register = value;
    }
}

// I/O Register handlers (NOTE: stubbed for now)
u8 io_read(GameBoy *gb, u16 addr) {
    // TODO: Implement I/O registers for each component
    // For now, return 0xFF (open bus)
    (void)gb;
    (void)addr;

    // Some registers have default values
    switch (addr) {
        case 0xFF00: // Joypad
            return 0xCF;
        case 0xFF40: // LCD Control
            return 0x91;
        case 0xFF47: // BG Palette
            return 0xFC;
        default:
            return 0xFF;
    }
}

void io_write(GameBoy *gb, u16 addr, u8 value) {
    // TODO: Implement I/O registers for each component
    // For now, just ignore writes
    (void)gb;
    (void)addr;
    (void)value;
}

// Debug Helper: Dump Memory Region
void mmu_dump_region(GameBoy *gb, u16 start, u16 end) {
    printf("Memory Dump [0x%04x - 0x%04x]:\n", start, end);

    for (u16 addr = start; addr <= end; addr += 16) {
        printf("0x%04x: ", addr);

        // Print B in hex
        for (int i = 0; i < 16 && (addr + 1) <= end; i++) {
            printf("%02x ", mmu_read(gb, addr + 1));
        }

        printf("\n");
    }
}
