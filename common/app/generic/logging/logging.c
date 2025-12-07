
#include "logging.h"

void logger_init(Logger* log, LogBuilder* builder, LogSubscriber* subs,
                 size_t num_subs, Send* comm)
{
    log->builder = builder;
    log->subs = subs;
    log->num_subs = num_subs;
    log->comm = comm;
    log->enabled = false;
}

bool logger_update(Logger* log)
{
    if (!log->enabled)
    {
        if (log->cmd_enable)
        {
            log->enabled = true;
            for (size_t i = 0; i < log->num_subs; ++i)
            {
                log->subs[i].clear(&log->subs[i]);
            }
        }
    }
    else
    {
        log->builder->build_new(log->builder);
        const uint8_t* data = log->builder->get_ptr(log->builder);
        size_t size = log->builder->get_size(log->builder);

        for (size_t i = 0; i < log->num_subs; ++i)
        {
            log->subs[i].write(&log->subs[i], data, size);
        }

        if (!log->cmd_enable)
        {
            log->enabled = false;
        }
    }

    return true;
}

void logger_enable(Logger* log, bool enable)
{
    log->cmd_enable = enable;
}

bool logger_retrieve(Logger* log)
{
    for (size_t i = 0; i < log->num_subs; ++i)
    {
        log->comm->fwrite(log->comm, "Start %zu:", i);
        log->subs[i].retrieve_all(&log->subs[i], log->comm);
        log->comm->fwrite(log->comm, "\nEnd %zu", i);
    }
    return true;
}