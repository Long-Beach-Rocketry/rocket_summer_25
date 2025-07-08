
#include "w25q_logger.h"
#include <string.h>

void W25qLoggerInit(LogSubscriber* sub, W25qLogger* flash_log, W25q* flash,
                    size_t max_index)
{
    flash_log->flash = flash;
    flash_log->end_char = 0xFF;
    flash_log->max_index = max_index;
    flash_log->allow_wrap = false;
    flash_log->index = 0;

    sub->priv = (void*)flash_log;
    sub->clear = W25qLoggerClear;
    sub->write = W25qLoggerWrite;
    sub->retrieve_all = W25qLoggerRetrieve;
}

void W25qLoggerWrapAround(LogSubscriber* sub, bool enable)
{
    W25qLogger* f_log = (W25qLogger*)sub->priv;
    f_log->allow_wrap = enable;
}

bool W25qLoggerClear(LogSubscriber* sub)
{
    W25qLogger* f_log = (W25qLogger*)sub->priv;
    f_log->index = 0;
    return true;
}

bool W25qLoggerWrite(LogSubscriber* sub, const uint8_t* data, size_t size)
{
    W25qLogger* f_log = (W25qLogger*)sub->priv;

    // Leave 1 byte to indicate end of record.
    if (size > f_log->flash->page_size - 1)
    {
        return false;
    }

    if (f_log->allow_wrap)
    {
        f_log->index = (f_log->index >= f_log->max_index) ? 0 : f_log->index;
    }

    // If sector boundary start, erase sector.
    if (!((f_log->index * f_log->flash->page_size) % f_log->flash->sector_size))
    {
        f_log->flash->erase_sector(f_log->flash,
                                   f_log->index * f_log->flash->page_size);
    }

    if (f_log->index < f_log->max_index)
    {
        f_log->flash->page_write(
            f_log->flash, f_log->index * f_log->flash->page_size, data, size);
        f_log->index++;

        return true;
    }

    return false;
}

bool W25qLoggerRetrieve(LogSubscriber* sub, Send* sender)
{
    W25qLogger* f_log = (W25qLogger*)sub->priv;

    static bool eof = false;

    if (f_log->flash->page_size > W25Q_MAX_PAGE_SIZE)
    {
        return false;
    }

    for (size_t page = 0; page < f_log->max_index; ++page)
    {
        uint8_t buf[W25Q_MAX_PAGE_SIZE] = {0};
        f_log->flash->read(f_log->flash, page * f_log->flash->page_size, buf,
                           f_log->flash->page_size);
        if (buf[0] == f_log->end_char)
        {
            if (!eof)
            {
                sender->fwrite(sender, "EOF");
                if (!f_log->allow_wrap)
                {
                    break;
                }
                eof = true;
            }
            continue;
        }
        size_t byte = 0;
        while (byte++ < W25Q_MAX_PAGE_SIZE)
        {
            if (buf[byte] == f_log->end_char)
            {
                break;
            }
        }
        buf[byte] = '\0';

        sender->fwrite(sender, "%s", (const char*)buf);
    }

    eof = false;
    return true;
}
