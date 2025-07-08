
#include "snx5176b.h"

bool GiveSnx5176b(Usart* iface, Mem* mem, Usart* uart, Gpio* txe, Gpio* rxe)
{
    EXIT_IF((uart == NULL) || (txe == NULL) || (rxe == NULL), false);
    Snx5176b* snx = ALLOC(mem, Snx5176b);
    EXIT_IF(snx == NULL, false);
    Snx5176bInit(iface, snx);
    return true;
}

Usart* MakeSnx5176b(Mem* mem, Usart* uart, Gpio* txe, Gpio* rxe)
{
    Usart* iface = ALLOC(mem, Usart);
    EXIT_IF(iface == NULL, NULL);
    EXIT_IF(!GiveSnx5176b(iface, mem, uart, txe, rxe), NULL);
    return iface;
}

void Snx5176bInit(Usart* rs485, Snx5176b* chip)
{
    rs485->priv = (void*)chip;
    rs485->send = Snx5176bSend;
    rs485->recv = Snx5176bRecv;
}

void Snx5176bConfig(Snx5176b* dev)
{
    Snx5176bTxSetEn(dev, false);
}

bool Snx5176bSend(Usart* rs485, const uint8_t* data, size_t size)
{
    Snx5176b* dev = (Snx5176b*)rs485->priv;
    Snx5176bTxSetEn(dev, true);
    bool success = dev->usart->send(dev->usart, data, size);
    Snx5176bTxSetEn(dev, false);
    return success;
}

bool Snx5176bRecv(Usart* rs485, uint8_t* data, size_t size)
{
    Snx5176b* dev = (Snx5176b*)rs485->priv;
    return dev->usart->recv(dev->usart, data, size);
}

void Snx5176bTxSetEn(Snx5176b* chip, bool enable)
{
    chip->rxe->set(chip->rxe, enable);
    chip->txe->set(chip->txe, enable);
}
