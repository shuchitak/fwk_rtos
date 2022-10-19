#include <platform.h>

#include "rtos_spi_slave.h"

#include "rtos_printf.h"
#include "device_control.h"

#define SPI_XFER_RX_SIZE (64)
#define SPI_XFER_TX_SIZE (64)
static uint8_t spi_xfer_rx_buf[SPI_XFER_RX_SIZE];
static uint8_t spi_xfer_tx_buf[SPI_XFER_TX_SIZE];

RTOS_SPI_SLAVE_CALLBACK_ATTR
void device_control_spi_start_cb(rtos_spi_slave_t *ctx,
                                 void *args)
{
    device_control_t *device_control_ctx = (device_control_t*)args;
    for(int i=0; i<8; i++)
    {
        spi_xfer_tx_buf[i] = 5;
    }
    spi_slave_xfer_prepare(ctx, spi_xfer_rx_buf, SPI_XFER_RX_SIZE, spi_xfer_tx_buf, SPI_XFER_TX_SIZE);

    control_ret_t dc_ret;
    rtos_printf("Registering SPI device control resources now\n");
    dc_ret = device_control_resources_register(device_control_ctx,
                                               pdMS_TO_TICKS(5000));

    if (dc_ret != CONTROL_SUCCESS) {
        rtos_printf("Device control resources failed to register for SPI on tile %d\n", THIS_XCORE_TILE);
    } else {
        rtos_printf("Device control resources registered for SPI on tile %d\n", THIS_XCORE_TILE);
    }
    xassert(dc_ret == CONTROL_SUCCESS);
}

RTOS_SPI_SLAVE_CALLBACK_ATTR
void device_control_spi_xfer_done_cb(rtos_spi_slave_t *ctx,
                                     void *args)
{
    device_control_t *device_control_ctx = (device_control_t*)args;
    uint8_t *rx_buf, *tx_buf;
    size_t rx_len, tx_len;

    spi_slave_xfer_complete(ctx, &rx_buf, &rx_len, &tx_buf, &tx_len, 0);

    if(rx_len < 3)
    {
        // Received packet length has to be atleast 3
        tx_buf[0] = CONTROL_MALFORMED_PACKET;
    }
    //Check for NOP as the first thing
    if ((rx_buf[0] == 0) && (rx_buf[1] == 0) && (rx_buf[2] == 0)) {
        // This is a NOP sent for reading tx_buf updated in the previous command.
        return;
    }
    
    control_ret_t ret;
    if (rx_len >= 3) {
        device_control_request(device_control_ctx,
                               rx_buf[0],
                               rx_buf[1],
                               rx_buf[2]);
        rx_len -= 3;
        ret = device_control_payload_transfer_bidir(device_control_ctx, &rx_buf[3], rx_len, tx_buf, &tx_len);
    }
}