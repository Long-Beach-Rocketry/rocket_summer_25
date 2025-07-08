
#include "prealloc.h"

static uint8_t* prealloced_mem;
static size_t mem_size;
static size_t idx;
static bool inited = false;

bool InitPrealloc(Mem* memory, uint8_t* data, size_t size)
{
    memory->alloc = Prealloc;
    if (!inited)
    {
        prealloced_mem = data;
        mem_size = size;
        idx = 0;
        inited = true;
        return true;
    }
    return false;
}

void* Prealloc(size_t size)
{
    if (!inited || ((idx + size) > mem_size))
    {
        return NULL;
    }
    uint8_t* curr_ptr = prealloced_mem + idx;
    idx += size;
    return (void*)curr_ptr;
}