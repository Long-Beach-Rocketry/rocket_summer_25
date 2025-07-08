
#include "dalloc.h"

bool enabled = false;

void InitDalloc(Mem* mem)
{
    mem->alloc = Dalloc;
}

void SetEnableDalloc(bool enable)
{
    enabled = enable;
}

void* Dalloc(size_t size)
{
    return (enabled) ? malloc(size) : NULL;
}