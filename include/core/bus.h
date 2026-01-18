#ifndef BUS_H
#define BUS_H

#include <core/utils.h>
#include <gbemu.h>

// ---------------------------------------------
// Memory read/write
// ---------------------------------------------
u8   mmu_read(GameBoy *gb, u16 addr);
void mmu_write(GameBoy *gb, u16 addr, u8 value);

// ---------------------------------------------
// Debug Helpers
// ---------------------------------------------
void mmu_dump_region(GameBoy *gb, u16 start, u16 end);

#endif // !BUS_H
