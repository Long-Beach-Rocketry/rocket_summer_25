
#include "altitude.h"

float altitude(float pressure_pa, float sea_level_pa)
{
    return 44330 * (1 - pow(pressure_pa / sea_level_pa, 1 / 5.255));
}
