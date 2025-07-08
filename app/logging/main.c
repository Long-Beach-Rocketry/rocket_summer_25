
#include "bulk_logger.h"
#include "logging.h"
#include "mock_nav_data.h"
#include "mock_w25q.h"
#include "nav_logbuilder.h"
#include "w25q_logger.h"

#include <stdio.h>

#define BULK_DATA_SIZE (256)

Logger logger;
LogBuilder builder;
LogSubscriber sub;
LogSubscriber bulk_sub;
BulkLogger bulk_priv;
uint8_t data[BULK_DATA_SIZE];
W25q flash;
W25qLogger flash_log;
NavDataLogBuilder nav_builder;
NavData nav;
MockNavData mock_nav;

bool logprint(const char* msg)
{
    printf("%s\n", msg);
    return true;
}

Send send;

int main(int argc, char* argv[])
{
    SendInit(&send, logprint);
    MockW25qInit(&flash);
    W25qLoggerInit(&sub, &flash_log, &flash, flash.mem_size / flash.page_size);
    W25qLoggerWrapAround(&sub, true);
    BulkLoggerInit(&bulk_sub, &bulk_priv, &sub, 4, '|', data, BULK_DATA_SIZE);
    MockNavDataInit(&nav, &mock_nav, "./navdata.log");
    NavDataLogBuilderInit(&builder, &nav_builder, &nav);
    logger_init(&logger, &builder, &bulk_sub, 1, &send);
    logger_update(&logger);
    logger_enable(&logger, true);
    logger_update(&logger);
    for (size_t i = 0; i < 18; ++i)
    {
        nav.update(&nav);
        logger_update(&logger);
        // MockW25qDumpMem(&send);
    }
    logger_retrieve(&logger);
    MockNavDataDeinit(&nav);
    return 0;
}
