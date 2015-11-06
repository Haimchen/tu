#include <stdint.h>
#include <stdio.h>
#include "mmu.h"


int16_t page_table[PAGE_COUNT];
sys_mode_t mode = KERNEL;


void switch_mode(sys_mode_t new_mode)
{
	if (new_mode == KERNEL && mode != new_mode) mode = KERNEL;
	else if(new_mode == USER && mode != new_mode) mode = USER;
}


// Flags:
// 1 x x x =  nicht existent
// 1 x x = available
// x 1 x = kernel only
// x x 1 = read-only

void init_page_table(void)
{
	int16_t i;
	int16_t adress;
	int16_t flag = 0;
	for (i = 0; i < PAGE_COUNT; i++) {
    // calculate adress from index
		adress = i * 16;

      // create flag depending on type of storage
			if (adress < 4096) flag = 4; // Kernel RAM
			else if(adress < 16384) flag = 4; // shared RAM
			else if (adress < 17408) flag = 8; // free
			else if (adress < 18432) flag = 4; // Flash
			else if (adress < 30720) flag = 8; // free
			else if (adress < 30724) flag = 4; // IO
			else if (adress < 32767) flag = 4; // free
			else printf("ERROR\n");

		page_table[i] = adress + flag;
	}


}


void remap(int16_t page, int16_t tile)
{
	if (mode == KERNEL) {
    // get flag only
		int16_t flag = page_table[page]%16;
		page_table[page] = 16 * tile + flag;
	}
}


void close_page(int16_t page)
{
	if (mode == KERNEL) {
      // get last 2 flags
			int16_t flag = page_table[page]%4;
			int16_t tile = page_table[page] - page_table[page]%8;
			page_table[page] = tile + flag;
	}
}


void open_page(int16_t page)
{
	if (mode == KERNEL) {
			int16_t flag = page_table[page]%4;
			int16_t tile = page_table[page] - page_table[page]%8;
			page_table[page] = tile + flag + 4;
	}
}


void page_kernel_only(int16_t page)
{
	if (mode == KERNEL) {
		if (page_table[page]%4 < 2)
			page_table[page] = page_table[page] + 2;
	}
}


void open_page_for_user(int16_t page)
{
	if (mode == KERNEL) {
		if (page_table[page]%4 >= 2)
			page_table[page] = page_table[page] - 2;
	}
}


void page_read_only(int16_t page)
{
	if (mode == KERNEL) {
		if (page_table[page]%2 == 0)
			page_table[page] = page_table[page] + 1;
	}
}


void page_read_write(int16_t page)
{
	if (mode == KERNEL) {
		if (page_table[page]%2 > 0)
			page_table[page] = page_table[page] - 1;
	}
}


int16_t memory_access(int16_t address, access_type operation)
{
  // special case: adress on IO page, but after IO range
  // address not in use
  if (address > 30723) {
    return INVALID_PHYSICAL_ADDRESS;
  }
  // get index from adress
	int16_t rest = address % 16;

  // get flags only
	int16_t flag = page_table[address/16] % 16;

	if (flag >= 8) return INVALID_PHYSICAL_ADDRESS;

	if (flag%8 < 4) return PAGE_UNAVAILABLE;

	if (flag%4 >= 2 && mode != KERNEL) return RIGHTS_MISSMATCH;

	if (flag%2 >= 1 && operation == WRITE) return RIGHTS_MISSMATCH;

	int16_t tile = page_table[address / 16];
	tile = tile - tile%16;

    return tile + rest;
}


void protect_system(void)
{
	int16_t i;
  // protect Kernel RAM
	for (i = 0; i < 256; i++) {

		page_kernel_only(i);
	}
  // protect IO
	page_kernel_only(30720 / 16);
}

