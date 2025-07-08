#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct network_if network_if;
struct network_if
{
    bool (*send)(uint8_t target_address, void* message);
    bool (*receive)(void* message);
};