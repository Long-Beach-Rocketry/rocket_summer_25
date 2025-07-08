
#include "bulk_logger.h"

void BulkLoggerInit(LogSubscriber* sub, BulkLogger* bulk_log,
                    LogSubscriber* internal, size_t divisor, uint8_t separator,
                    uint8_t* backing_buf, size_t buf_size)
{
    bulk_log->divisor = divisor;
    bulk_log->seperator = separator;
    bulk_log->buf = backing_buf;
    bulk_log->buf_size = buf_size;
    bulk_log->curr_iters = 0;
    bulk_log->internal = internal;

    sub->priv = (void*)bulk_log;
    sub->clear = BulkLoggerClear;
    sub->write = BulkLoggerWrite;
    sub->retrieve_all = BulkLoggerRetrieve;
}

bool BulkLoggerClear(LogSubscriber* sub)
{
    BulkLogger* bulk_log = (BulkLogger*)sub->priv;
    return bulk_log->internal->clear(bulk_log->internal);
}

bool BulkLoggerWrite(LogSubscriber* sub, const uint8_t* data, size_t size)
{
    BulkLogger* bulk_log = (BulkLogger*)sub->priv;

    bool success = false;
    // Check if iterations remain to buffer.
    if (bulk_log->curr_iters < bulk_log->divisor)
    {
        // Fail if out of backing memory.
        if (size > (bulk_log->buf_size - bulk_log->buf_idx))
        {
            return false;
        }
        memcpy(bulk_log->buf + bulk_log->buf_idx, data, size);
        bulk_log->buf_idx += size;
        bulk_log->buf[bulk_log->buf_idx++] = bulk_log->seperator;
        bulk_log->curr_iters++;
    }
    else
    {
        // Write to internal log.
        success = bulk_log->internal->write(bulk_log->internal, bulk_log->buf,
                                            bulk_log->buf_idx);
        memset(bulk_log->buf, 0, bulk_log->buf_idx);
        bulk_log->curr_iters = 0;
        bulk_log->buf_idx = 0;
    }

    return success;
}

bool BulkLoggerRetrieve(LogSubscriber* sub, Send* sender)
{
    BulkLogger* bulk_log = (BulkLogger*)sub->priv;
    return bulk_log->internal->retrieve_all(bulk_log->internal, sender);
}
