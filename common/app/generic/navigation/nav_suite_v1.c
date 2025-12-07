
#include "nav_suite_v1.h"

void NavSuiteV1Init(NavData* data, NavSuiteV1* bosch, Bmp390* baro, Bno055* imu)
{
    bosch->baro = baro;
    bosch->imu = imu;
    data->priv = (void*)bosch;

    data->update = NavSuiteV1Update;
}
/**
 * Updates navdata 
 */
bool NavSuiteV1Update(NavData* data)
{
    NavSuiteV1* nav = (NavSuiteV1*)data->priv;

    data->pressure = nav->baro->get_pressure_pa(nav->baro);
    data->temperature = nav->baro->get_temp_c(nav->baro);

    nav->imu->get_accel(nav->imu, &data->accel);
    nav->imu->get_quaternion(nav->imu, &data->quat);

    return true;
}
