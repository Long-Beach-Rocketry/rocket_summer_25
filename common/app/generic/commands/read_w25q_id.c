
#include "read_w25q_id.h"

static Send* send;
static Spi* dev;

void init_read_w25q_id(Send* sender, Spi* spi)
{
    send = sender;
    dev = spi;
}

void read_w25q_id(int argc, char* argv[])
{
    uint8_t tx[6] = {0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t rx[6] = {0};

    dev->cs->select(dev->cs);
    dev->transact(dev, tx, rx, 4);
    dev->cs->deselect(dev->cs);

    send->fwrite(send, "%x %x %x", rx[1], rx[2], rx[3]);
}