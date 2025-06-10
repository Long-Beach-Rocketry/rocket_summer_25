#include "count_debounce.h"

void CountDebounce_Init(CountDebounce* db, IoInput* input, Gpio* gpio,
                        uint8_t threshold)
{
    db->state = DEBOUNCE_OFF;
    db->counter = 0;
    db->threshold = threshold;
    db->gpio = gpio;

    input->get_state = CountDebounce_GetStateFromIoInput;
    input->priv = (void*)db;
}

bool CountDebounce_GetStateFromIoInput(IoInput* input)
{
    CountDebounce* db = (CountDebounce*)input->priv;
    bool raw = db->gpio->read(db->gpio);

    if ((raw && db->state == DEBOUNCE_OFF) ||
        (!raw && db->state == DEBOUNCE_ON))
    {
        db->counter++;
        if (db->counter >= db->threshold)
        {
            db->state = raw ? DEBOUNCE_ON : DEBOUNCE_OFF;
            db->counter = 0;
        }
    }
    else
    {
        db->counter = 0;
    }
    return db->state == DEBOUNCE_ON;
}

DebounceState CountDebounce_GetState(CountDebounce* db)
{
    return db->state;
}

bool CountDebounce_IsPressed(CountDebounce* db)
{
    return db->state == DEBOUNCE_ON;
}

bool CountDebounce_IsReleased(CountDebounce* db)
{
    return db->state == DEBOUNCE_OFF;
}