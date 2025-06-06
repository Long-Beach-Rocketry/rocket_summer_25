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

    dev->instance->CR1 = 0;

    /* 
     * Enable Software slave management so we ignore the NSS pin and control through software
     * Set internal NSS to high
    */
    dev->instance->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);

    /* Set MSB first */
    dev->instance->CR1 &= ~SPI_CR1_LSBFIRST;

    /** 
      * L4 SPI clock configuration
      * Set baudrate prescaler to balance speed and power consumption
      * SPI Clock = PCLK / 2^(BR+1)
     */

    /* Master mode */
    dev->instance->CR1 |= SPI_CR1_MSTR;

    /* CR2 clear */
    dev->instance->CR2 = 0;

    /* Set FIFO reception threshold to 8 bits */
    dev->instance->CR2 |= SPI_CR2_FRXTH;

    dev->instance->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);

    /* Enable SPI */
    dev->instance->CR1 |= SPI_CR1_SPE;
}

bool StSpiSend(Spi* spi, const uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    /* Check busy flag */
    if (dev->instance->SR & SPI_SR_BSY)
    {
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        /* Wait for transmit buffer is empty */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /* 
         * L4 SPI data transmission 
         * Writing to DR automatically starts clock generation
         * The L4's FIFO handles buffering to maintain continuous transfers
         */
        /* The cast is because the register is 16 bits but we are only using 8*/
        *(DataReg)&dev->instance->DR = data[i++];

        /* Wait for receive buffer to be full */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /* 
         * Discard received data 
         */
        (*(DataReg)&dev->instance->DR);
    }

    /* Ensure transmission fully completes before returning */
    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}

bool StSpiRead(Spi* spi, uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    /* Check peripheral status */
    if (dev->instance->SR & SPI_SR_BSY)
    {
        /* L4 peripheral busy - reject operation */
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        /* L4 TX FIFO status check */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /* 
         * Send dummy byte (0x00) to generate clock pulses
         * On L4, each byte sent generates exactly one byte received
         */
        *(DataReg)&dev->instance->DR = 0;

        /* Wait for L4's RX FIFO to have data available */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /* 
         * Read actual data from slave device
         * L4's DR read automatically manages FIFO pointers
         */
        data[i++] = *(DataReg)&dev->instance->DR;
    }

    /* Ensure L4 bus is idle before returning */
    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}

bool StSpiTransact(Spi* spi, const uint8_t* txdata, uint8_t* rxdata,
                   size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    /* Check if line is busy, or if CS fails */
    if (dev->instance->SR & SPI_SR_BSY)
    {
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        /* L4 FIFO management - ensure space for TX data */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /* 
         * Initiate full-duplex data exchange
         * The L4 SPI peripheral handles simultaneous TX/RX
         */
        *(DataReg)&dev->instance->DR = txdata[i];

        /* Monitor L4's RXNE flag for incoming data */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /* 
         * Store received byte while advancing index
         * The L4 ensures perfect synchronization between TX and RX
         */
        rxdata[i++] = *(DataReg)&dev->instance->DR;
    }

    /* L4 bus completion check */
    WAIT(dev->timer, !(dev->instance->SR & SPI_SR_BSY), false);

    return true;
}