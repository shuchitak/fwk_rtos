#ifndef SPI_DEVICE_CONTROL_H_
#define SPI_DEVICE_CONTROL_H_

#include "rtos_spi_slave.h"
#include "device_control.h"
#ifdef __cplusplus
extern "C" {
#endif
void device_control_spi_start_cb(rtos_spi_slave_t *ctx,
                                 void *args);

void device_control_spi_xfer_done_cb(rtos_spi_slave_t *ctx,
                                     void *args);
#ifdef __cplusplus
}
#endif
#endif                    
