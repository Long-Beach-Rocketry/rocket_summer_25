
#include "helios.h"

bool HeliosNavigatorInit(Navigator* nav, Helios* helios, NavData* nav_data)
{
    if (nav == NULL || helios == NULL || nav_data == NULL ||
        nav_data->update == NULL)
    {
        return false;
    }

    helios->data = nav_data;
    helios->started = false;
    nav->priv = (void*)helios;

    nav->altitude = HeliosAltitude;
    nav->orientation = HeliosOrientation;
    nav->start = HeliosStart;
    nav->update = HeliosUpdate;
    nav->velocity = HeliosVelocity;
    nav->acceleration = HeliosAcceleration;

    return true;
}

bool HeliosStart(Navigator* nav)
{
    if (nav == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    if (p->kalman.model->A == NULL || p->kalman.model->H == NULL ||
        p->kalman.model->Q == NULL || p->kalman.model->R == NULL ||
        p->kalman.x == NULL || p->kalman.P == NULL)
    {
        return false;
    }

    if (!p->data->update(p->data))
    {
        return false;
    }

    p->base_pressure = p->data->pressure;
    p->started = true;

    return true;
}

bool HeliosUpdate(Navigator* nav)
{
    if (nav == NULL)
    {
        return false;
    }

    Helios* p = (Helios*)nav->priv;

    if (!p->started)
    {
        return false;
    }

    if (!SimpleKalmanPredict(&p->kalman))
    {
        return false;
    }

    if (!p->data->update(p->data))
    {
        return false;
    }

    // Use starting pressure as sea level to measure relative altitude.
    double curr_altitude = altitude(p->data->pressure, p->base_pressure);

    /**
     * Rotate the raw 3-axis acceleration from sensor frame to world frame.
     * The IMU provides us with a quaternion reading, so we can use it to help.
     */
    ThreeAxisVec corrected_accel;
    accel_rotate(&p->data->accel, &p->data->quat, &corrected_accel);
    double curr_vert_accel = corrected_accel.z - EARTH_GRAVITY_M_S2;

    // Update Kalman filter.
    MATRIX(z, 2, 1, {curr_altitude}, {curr_vert_accel});
    bool s = SimpleKalmanEstimate(&p->kalman, &z);

    return s;
}

double HeliosAltitude(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return MAT_GET(p->kalman.x, 0, 0);
}

double HeliosVelocity(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return MAT_GET(p->kalman.x, 1, 0);
}

double HeliosAcceleration(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return MAT_GET(p->kalman.x, 2, 0);
}

const QuaternionVec* HeliosOrientation(Navigator* nav)
{
    Helios* p = (Helios*)nav->priv;
    return (const QuaternionVec*)&p->data->quat;
}