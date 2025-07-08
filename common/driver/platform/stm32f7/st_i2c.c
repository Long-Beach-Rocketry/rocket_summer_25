
#include "st_i2c.h"

void StI2cInit(I2c* i2c, StPrivI2c* st_i2c, uint32_t base_addr, Timeout* timer)
{
    st_i2c->instance = (I2C_TypeDef*)base_addr;
    st_i2c->timer = timer;

    i2c->priv = (void*)st_i2c;
    i2c->write = StI2cWrite;
    i2c->read = StI2cRead;
    i2c->set_target = StI2cSetTarget;
}

void StI2cConfig(I2c* i2c, uint32_t timingr)
{
    StPrivI2c* dev = (StPrivI2c*)i2c->priv;

    /*
     * Disable I2C, clear register.
     */
    dev->instance->CR1 &= 0x0;

    /*
     * Set timing.
     */
    dev->instance->TIMINGR |= timingr;
    dev->instance->TIMINGR &= timingr;

    /*
     * Enable I2C.
     */
    dev->instance->CR1 |= I2C_CR1_PE;
}

bool StI2cSetTarget(I2c* i2c, uint8_t dev_addr)
{
    StPrivI2c* dev = (StPrivI2c*)i2c->priv;
    /*
     * Check if transfer in progress.
     */
    if (dev->instance->CR2 & (I2C_CR2_START))
    {
        return false;
    }

    /*
     * Clear register.
     */
    dev->instance->CR2 &= ~I2C_CR2_SADD;
    dev->instance->CR2 |= dev_addr;

    return true;
}

bool StI2cWrite(I2c* i2c, uint16_t addr, const uint8_t* data, size_t size)
{
    StPrivI2c* dev = (StPrivI2c*)i2c->priv;

    /*
     * Check if transfer in progress.
     */
    if (dev->instance->CR2 & (I2C_CR2_START))
    {
        return false;
    }

    /*
     * Read direction, bit = 0;
     * Clear number of bytes.
     */
    dev->instance->CR2 &= ~(I2C_CR2_RD_WRN | I2C_CR2_NBYTES);

    /*
     * Configure for transfer:
     * Set size (data size + mem address), set autoend,
     * send start bit.
     */
    dev->instance->CR2 |=
        (((size + 1) << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START);

    /*
     * Request write.
     */
    WAIT(dev->timer, dev->instance->ISR & I2C_ISR_TXE, false);
    dev->instance->TXDR = addr;

    /*
     * Send data.
     */
    for (size_t i = 0; i < size;)
    {

        /*
         * NAK.
         */
        if (dev->instance->ISR & I2C_ISR_NACKF)
        {
            dev->instance->ICR |= I2C_ICR_NACKCF;
            return false;
        }

        /*
         * Ready for transfer.
         */
        WAIT(dev->timer, dev->instance->ISR & I2C_ISR_TXIS, false);
        dev->instance->TXDR = data[i++];
    }

    /*
     * Detect stop.
     */
    WAIT(dev->timer, dev->instance->ISR & I2C_ISR_STOPF, false);
    dev->instance->ICR |= I2C_ICR_STOPCF;

    return true;
}

bool StI2cRead(I2c* i2c, uint16_t addr, uint8_t* data, size_t size)
{
    StPrivI2c* dev = (StPrivI2c*)i2c->priv;

    /*
     * Check if transfer in progress.
     */
    if (dev->instance->CR2 & (I2C_CR2_START))
    {
        return false;
    }

    /*
     * Set transfer direction (w = 0),
     * softend.
     *
     * Create start, set size to 1 for sending
     * start address.
     */
    dev->instance->CR2 &= ~(I2C_CR2_NBYTES | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN);
    dev->instance->CR2 |= (1 << I2C_CR2_NBYTES_Pos | I2C_CR2_START);

    /*
     * Request read.
     */
    WAIT(dev->timer, dev->instance->ISR & I2C_ISR_TXE, false);
    dev->instance->TXDR = addr;

    /*
     * Wait for transfer complete.
     */
    WAIT(dev->timer, dev->instance->ISR & I2C_ISR_TC, false);

    /*
     * Configure for read operation, size to
     * data size, autoend mode, and send start.
     */
    dev->instance->CR2 &= ~I2C_CR2_NBYTES_Msk;
    dev->instance->CR2 |= (I2C_CR2_RD_WRN | size << I2C_CR2_NBYTES_Pos |
                           I2C_CR2_AUTOEND | I2C_CR2_START);

    /*
     * Fill read buffer.
     */
    for (size_t i = 0; i < size;)
    {
        WAIT(dev->timer, dev->instance->ISR & I2C_ISR_RXNE, false);
        data[i++] = dev->instance->RXDR;
    }

    /*
     * Detect stop.
     */
    WAIT(dev->timer, dev->instance->ISR & I2C_ISR_STOPF, false);
    dev->instance->ICR |= I2C_ICR_STOPCF;

    return true;
}
