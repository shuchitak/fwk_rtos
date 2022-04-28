#!/usr/bin/env python
# Copyright 2022 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

from uart_rx_checker import UARTRxChecker, Parity
from pathlib import Path
import Pyxsim as px
import pytest

speed_args = {"1843200 baud": 1843200,
              "921600 baud": 921600,
              "576000 baud": 576000,
              "115200 baud": 115200,
              "38400 baud": 38400,
              "19200 baud": 19200,
              "9600 baud": 9600,
              "1200 baud": 1200}

data_bit_args = {   "five": 5,
                    "six": 6,
                    "seven": 7,
                    "eight": 8}

parity_args = { "NONE": 0,
                "EVEN": 1,
                "ODD": 2}                

stop_bit_args = {   "one": 1,
                    "two": 2}


@pytest.mark.parametrize("baud", speed_args.values(), ids=speed_args.keys())
@pytest.mark.parametrize("data", data_bit_args.values(), ids=data_bit_args.keys())
@pytest.mark.parametrize("parity", parity_args.values(), ids=parity_args.keys())
@pytest.mark.parametrize("stop", stop_bit_args.values(), ids=stop_bit_args.keys())
def test_uart_rx(baud, data, parity, stop):
    myenv = {'baud': baud}
    cwd = Path(request.fspath).parent

    resources = xmostest.request_resource("xsim")

    checker = UARTRxChecker("tile[0]:XS1_PORT_1A", "tile[0]:XS1_PORT_1B", parity, baud, stop, data)


    xmostest.run_on_simulator(resources['xsim'],
                              'app_uart_test_rx/bin/smoke/app_uart_test_rx_smoke.xe',
                              simthreads=[checker],
                              xscope_io=True,
                              tester=tester,
                              simargs=["--vcd-tracing", "-tile tile[0] -ports -o trace.vcd"],
                              clean_before_build=True,
                              build_env=myenv)


    px.run_with_pyxsim(binary,
                    simthreads = [checker],
                    simargs = sim_args)

    tester.run(capfd.readouterr().out)
