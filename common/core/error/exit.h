/**
 * Function exit handling.
 */

#pragma once

#define EXIT_IF(cond, ret) \
    do                     \
    {                      \
        if (cond)          \
        {                  \
            return ret;    \
        }                  \
    } while (0)
