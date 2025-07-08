
#include "nav_logbuilder.h"

void NavDataLogBuilderInit(LogBuilder* builder, NavDataLogBuilder* nav_builder,
                           const NavData* data)
{
    nav_builder->data = data;
    nav_builder->size = 0;
    memset(nav_builder->buffer, 0, NAV_DATA_MAX_SIZE_BYTES);
    builder->priv = (void*)nav_builder;
    builder->build_new = NavDataLogBuilderBuildNew;
    builder->get_ptr = NavDataLogBuilderGetPtr;
    builder->get_size = NavDataLogBuilderGetSize;
}

bool NavDataLogBuilderBuildNew(LogBuilder* builder)
{
    NavDataLogBuilder* nav_builder = (NavDataLogBuilder*)builder->priv;
    nav_builder->size =
        (size_t)sprintf((char*)nav_builder->buffer,
                        "%zu,%.02f,%.02f,%.02f,%.02f,%.02f,%.02f,%.02f,%.02f",
                        nav_builder->data->tick, nav_builder->data->pressure,
                        nav_builder->data->accel.x, nav_builder->data->accel.y,
                        nav_builder->data->accel.z, nav_builder->data->quat.w,
                        nav_builder->data->quat.x, nav_builder->data->quat.y,
                        nav_builder->data->quat.z);

    return true;
}

const uint8_t* NavDataLogBuilderGetPtr(LogBuilder* builder)
{
    NavDataLogBuilder* nav_builder = (NavDataLogBuilder*)builder->priv;
    return nav_builder->buffer;
}

size_t NavDataLogBuilderGetSize(LogBuilder* builder)
{
    NavDataLogBuilder* nav_builder = (NavDataLogBuilder*)builder->priv;
    return nav_builder->size;
}