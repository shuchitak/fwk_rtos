// Copyright (c) 2021-2022 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#ifndef I2C_DEVICE_CONTROL_H_
#define I2C_DEVICE_CONTROL_H_

#include "rtos_i2c_slave.h"
#include "device_control.h"

#ifdef __cplusplus
extern "C" {
#endif
void device_control_i2c_start_cb(rtos_i2c_slave_t *ctx,
                                 void *args);

void device_control_i2c_rx_cb(rtos_i2c_slave_t *ctx,
                              void *args,
                              uint8_t *data,
                              size_t len);

size_t device_control_i2c_tx_start_cb(rtos_i2c_slave_t *ctx,
                                      void *args,
                                      uint8_t **data);

#ifdef __cplusplus
}
#endif
#endif /* I2C_DEVICE_CONTROL_H_ */
