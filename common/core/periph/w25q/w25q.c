
#include "w25q.h"

size_t get_section_addr(size_t address, size_t section_size)
{
    return address - (address % section_size);
}

static bool mask_status(W25q* flash, uint8_t cmd, uint8_t mask)
{
    // Chip select must already be asserted prior to function call.
    uint8_t status = 0;
    flash->bus->send(flash->bus, &cmd, 1);
    flash->bus->read(flash->bus, &status, 1);
    return status & mask;
}

static void wait_busy(W25q* flash)
{
    // Wait busy bit in status register 1, 0x5
    flash->bus->cs->select(flash->bus->cs);
    while (mask_status(flash, 0x5, 1));
    flash->bus->cs->deselect(flash->bus->cs);
}

static void write_enable(W25q* flash)
{
    // Write enable instruction 0x6
    uint8_t write_en = 0x6;
    flash->bus->cs->select(flash->bus->cs);
    flash->bus->send(flash->bus, &write_en, 1);
    flash->bus->cs->deselect(flash->bus->cs);
}

void W25qInit(W25q* flash, Spi* spi, size_t mem_size)
{
    flash->bus = spi;
    flash->page_write = W25qPageWrite;
    flash->read = W25qRead;
    flash->erase_sector = W25qSectorErase;
    flash->page_size = W25Q_PAGE_SIZE_BYTES;
    flash->sector_size = W25Q_SECTOR_SIZE_BYTES;
    flash->mem_size = mem_size;
}

bool W25qPageWrite(W25q* flash, size_t address, const uint8_t* data,
                   size_t size)
{
    // Enforce start addr + size < next sector addr, so no wrap around happens.
    if ((address + size) >
        (get_section_addr(address, flash->page_size) + flash->page_size))
    {
        return false;
    }

    write_enable(flash);

    // Prog Page 0x2 + 24 bit address
    uint8_t txbuf[4] = {0x2, address >> 16, (address >> 8) & 0xFF,
                        address & 0xFF};
    uint8_t rxbuf[256] = {0};
    flash->bus->cs->select(flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->transact(flash->bus, data, rxbuf, size);
    flash->bus->cs->deselect(flash->bus->cs);

    // Wait busy bit in status register 1, 0x5
    wait_busy(flash);

    return true;
}

bool W25qRead(W25q* flash, size_t address, uint8_t* data, size_t size)
{
    wait_busy(flash);

    // Write enable instruction 0x6
    uint8_t write_en = 0x6;
    flash->bus->cs->select(flash->bus->cs);
    flash->bus->send(flash->bus, &write_en, 1);
    flash->bus->cs->deselect(flash->bus->cs);

    // Read data instruction 0x3
    uint8_t txbuf[4] = {0x3, address >> 16, (address >> 8) & 0xFF,
                        address & 0xFF};
    flash->bus->cs->select(flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->read(flash->bus, data, size);
    flash->bus->cs->deselect(flash->bus->cs);

    return true;
}

bool W25qSectorErase(W25q* flash, size_t address)
{
    address = get_section_addr(address, flash->sector_size);

    write_enable(flash);

    // Sector erase cmd 0x20 followed by 24-bit address
    uint8_t txbuf[4] = {0x20, address >> 16, (address >> 8) & 0xFF,
                        address & 0xFF};
    flash->bus->cs->select(flash->bus->cs);
    flash->bus->send(flash->bus, txbuf, 4);
    flash->bus->cs->deselect(flash->bus->cs);

    // Wait busy bit in status register 1, 0x5
    wait_busy(flash);

    return true;
}
