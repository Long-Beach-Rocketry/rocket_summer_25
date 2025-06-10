/**
 * 
 * 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct Gpio Gpio;

struct Gpio
{
    void (*config)(Gpio* gpio);
    bool (*toggle)(Gpio* gpio);
    void (*set)(Gpio* gpio, bool active);
    bool (*read)(Gpio* gpio);
    void* priv;
};