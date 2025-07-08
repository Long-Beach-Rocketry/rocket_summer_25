
#include "st_spi.h"

typedef volatile uint8_t* DataReg;

void StSpiInit(Spi* spi, StPrivSpi* st_spi, uint32_t base_addr,
               uint32_t timeout_ms)
{
    st_spi->timeout_ms = timeout_ms;

    st_spi->handle.Instance = (SPI_TypeDef*)base_addr;
    st_spi->handle.Init.Mode = SPI_MODE_MASTER;
    st_spi->handle.Init.Direction = SPI_DIRECTION_2LINES;
    st_spi->handle.Init.DataSize = SPI_DATASIZE_8BIT;
    st_spi->handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    st_spi->handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    st_spi->handle.Init.NSS = SPI_NSS_SOFT;
    st_spi->handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    st_spi->handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    st_spi->handle.Init.TIMode = SPI_TIMODE_DISABLE;
    st_spi->handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    st_spi->handle.Init.CRCPolynomial = 0x7;
    st_spi->handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    st_spi->handle.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
    st_spi->handle.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
    st_spi->handle.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
    st_spi->handle.Init.MasterInterDataIdleness =
        SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
    st_spi->handle.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
    st_spi->handle.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    st_spi->handle.Init.IOSwap = SPI_IO_SWAP_DISABLE;
    st_spi->handle.Init.ReadyMasterManagement =
        SPI_RDY_MASTER_MANAGEMENT_INTERNALLY;
    st_spi->handle.Init.ReadyPolarity = SPI_RDY_POLARITY_HIGH;
    st_spi->handle.State = HAL_SPI_STATE_RESET;

    spi->priv = (void*)st_spi;
    spi->send = StSpiSend;
    spi->read = StSpiRead;
    spi->transact = StSpiTransact;
}

void StSpiConfig(Spi* spi)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    dev->mosi.config(&dev->mosi);
    dev->miso.config(&dev->miso);
    dev->scl.config(&dev->scl);

    HAL_SPI_Init(&dev->handle);
}

bool StSpiSend(Spi* spi, const uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    HAL_StatusTypeDef status =
        HAL_SPI_Transmit(&dev->handle, data, size, dev->timeout_ms);

    return status == HAL_OK;
}

bool StSpiRead(Spi* spi, uint8_t* data, size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    HAL_StatusTypeDef status =
        HAL_SPI_Receive(&dev->handle, data, size, dev->timeout_ms);

    return status == HAL_OK;
}

bool StSpiTransact(Spi* spi, const uint8_t* txdata, uint8_t* rxdata,
                   size_t size)
{
    StPrivSpi* dev = (StPrivSpi*)spi->priv;

    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(
        &dev->handle, txdata, rxdata, size, dev->timeout_ms);

    return status == HAL_OK;
}
