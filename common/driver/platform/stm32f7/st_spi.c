#include "st_spi.h"

typedef volatile uint8_t* DataReg;

void StSpiInit(Spi* spi, StPrivSpi* st_spi, uint32_t base_addr, Timeout* timer)
{
    st_spi->instance = (SPI_TypeDef*)base_addr;
    st_spi->timer = timer;
    spi->priv = (void*)st_spi;
    spi->send = StSpiSend;
    spi->read = StSpiRead;
    spi->transact = StSpiTransact;
}

void StSpiConfig(Spi* spi)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    // Disable SPI and turn off: Bidirectional mode, CRC, RX only
    // set CPOL/CPHA = 0, 8-bit
    dev->instance->CR1 &= 0;

    // SSI bit usage off
    dev->instance->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);

    // 0 = MSB first 1 = LSB first
    dev->instance->CR1 &= ~SPI_CR1_LSBFIRST;

    /** 
     * Set bitrate: freq pclk / 2 ^ (BR + 1)
     */
    dev->instance->CR1 |= 0;

    dev->instance->CR1 |= SPI_CR1_MSTR;  // Master mode

    // Reset reg to set motorola mode, disable SS
    dev->instance->CR2 &= 0;
    dev->instance->CR2 |= SPI_CR2_FRXTH;  // 0 = 16 bit 1 = 8bit
    dev->instance->CR2 |=
        (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);  // 8bit, 0111

    // Enable SPI
    dev->instance->CR1 |= SPI_CR1_SPE;
}

bool StSpiSend(Spi* spi, const uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    // Check if line is busy, or if CS fails
    if (dev->instance->SR & SPI_SR_BSY)
    {
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->DR = data[i++];

        // Wait for RX FIFO to fill
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /** 
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
        *(DataReg)&dev->instance->DR;
    }

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}

bool StSpiRead(Spi* spi, uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    // Check if line is busy, or if CS fails
    if (dev->instance->SR & SPI_SR_BSY)
    {
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->DR = 0;

        // Wait for RX FIFO to fill
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /** 
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
        data[i++] = *(DataReg)&dev->instance->DR;
    }

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}

bool StSpiTransact(Spi* spi, const uint8_t* txdata, uint8_t* rxdata,
                   size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    // Check if line is busy, or if CS fails
    if (dev->instance->SR & SPI_SR_BSY)
    {
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        // Wait for space in TX FIFO
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /**
         * Send: Fill TXFIFO (by writing to SPI_DR)
         * When transmission is enabled, seq begins and continues
         * while data present in FIFO. CLK ends when FIFO becomes empty
         */
        *(DataReg)&dev->instance->DR = txdata[i];

        // Wait for RX FIFO to fill
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /** 
         * Read access to SPI_DR returns oldest value stored in RXFIFO not yet read
         * Write access to SPI_DR stores data in TXFIFO at end of queue
         * Must be aligned with RXFIFO threshold conf by FRXTH, FTLVL and FRLVL indicate FIFO occupancy
         */
        rxdata[i++] = *(DataReg)&dev->instance->DR;
    }

    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}
