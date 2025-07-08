#include "st_spi.h"

/**
 * STM32L4 SPI Driver
 * 
 * The L4 series features enhanced low-power SPI peripherals with FIFO buffers.
 * This implementation focuses on reliability and power efficiency.
 */

/* Data register access type - 8-bit for byte-oriented transfers */
typedef volatile uint8_t* DataReg;

void StSpiInit(Spi* spi, StPrivSpi* st_spi, uint32_t base_addr, Timeout* timer)
{
    /* Map hardware instance to the L4's peripheral address space */
    st_spi->instance = (SPI_TypeDef*)base_addr;
    st_spi->timer = timer;

    /* Connect hardware-specific implementation to generic interface */
    spi->priv = (void*)st_spi;
    spi->send = StSpiSend;
    spi->read = StSpiRead;
    spi->transact = StSpiTransact;
}

void StSpiConfig(Spi* spi)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    /* L4 GPIO configuration - ensure appropriate alternate function mapping */
    dev->mosi.config(&dev->mosi);
    dev->miso.config(&dev->miso);
    dev->scl.config(&dev->scl);

    /* 
     * STM32L4 SPI Configuration
     * ------------------------
     * The L4 series operates in low-power mode by default
     */

    /* Full reset of CR1 register before configuration */
    dev->instance->CR1 = 0;

    /* Enable internal slave select with software management for better power efficiency */
    dev->instance->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);

    /* Use MSB first for standard compatibility with most SPI devices */
    dev->instance->CR1 &= ~SPI_CR1_LSBFIRST;

    /* 
     * L4 SPI clock configuration
     * Set baudrate prescaler to balance speed and power consumption
     * SPI Clock = PCLK / 2^(BR+1)
     */
    dev->instance->CR1 |= 0; /* Default to highest speed (BR=000) */

    /* Configure as master device - L4 supports both master and slave modes */
    dev->instance->CR1 |= SPI_CR1_MSTR;

    /* CR2 configuration for the L4 peripheral */
    dev->instance->CR2 = 0;

    /* 
     * L4 FIFO threshold setting - critical for small data packets
     * Set to trigger on 8-bit to optimize power consumption for byte transfers
     */
    dev->instance->CR2 |= SPI_CR2_FRXTH;

    /* 
     * L4 data frame configuration - 8-bit data size (0111)
     * The L4 supports data sizes from 4 to 16 bits
     */
    dev->instance->CR2 |= (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2);

    /* Enable the SPI peripheral and put it in active state */
    dev->instance->CR1 |= SPI_CR1_SPE;
}

bool StSpiSend(Spi* spi, const uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    /* Verify peripheral is ready for transmission */
    if (dev->instance->SR & SPI_SR_BSY)
    {
        /* L4 peripheral is busy - cannot initiate new transfer */
        return false;
    }

    for (size_t i = 0; i < size;)
    {
        /* Wait for TX FIFO space availability - prevents overrun */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_TXE, false);

        /* 
         * L4 SPI data transmission 
         * Writing to DR automatically starts clock generation
         * The L4's FIFO handles buffering to maintain continuous transfers
         */
        *(DataReg)&dev->instance->DR = data[i++];

        /* 
         * Wait for RX data - required even when only transmitting
         * The L4 requires reading the received data to clear RXNE flag
         */
        WAIT(dev->timer, dev->instance->SR & SPI_SR_RXNE, false);

        /* 
         * Discard received data - read necessary to maintain FIFO flow
         * This read clears the RXNE flag and advances the FIFO pointer
         */
        (void)(*(DataReg)&dev->instance->DR);
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

    /* Verify L4 peripheral is ready */
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