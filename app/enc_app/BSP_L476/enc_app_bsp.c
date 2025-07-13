#include "enc_app_bsp.h"

void BSP_Init(QEnc* enc, StQenc* st_enc, size_t base_address)
{
    NidecEncInit(enc, st_enc, base_address);
}