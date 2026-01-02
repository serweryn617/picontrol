#!/usr/bin/env python3

import argparse
import struct
import time
from random import randbytes

import defs
from command import enter_bootsel
from flash_commands import flash_chip_erase, flash_program, flash_read, flash_read_status, flash_sector_erase
from gpio_commands import gpio_get, gpio_set, gpio_set_high_z
from i2c_commands import i2c_check_ack, i2c_read, i2c_set_address, i2c_set_speed, i2c_set_timeout, i2c_write
from serial_comm import SerialCommunicator
from spi_commands import spi_cs_deselect, spi_cs_select, spi_read, spi_set_speed, spi_write


def make_gpio_masks(gpios_on, gpios_off):
    pin_mask = 0
    pin_values = 0

    if gpios_on:
        for pin in gpios_on:
            pin_mask |= 1 << pin
            pin_values |= 1 << pin

    if gpios_off:
        for pin in gpios_off:
            pin_mask |= 1 << pin
            pin_values &= ~(1 << pin)

    return pin_mask, pin_values


def number(x):
    return int(x, 0)


def main():
    parser = argparse.ArgumentParser(description="Control GPIOs via USB device.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("enter_bootsel", help="Enter Pico BOOTSEL mode")

    gpio_parser = subparsers.add_parser("gpio", help="Control GPIO pins")
    gpio_subparsers = gpio_parser.add_subparsers(dest="gpio_command", required=True)
    gpio_set_parser = gpio_subparsers.add_parser("set", help="Set GPIO pins")
    gpio_set_parser.add_argument("--on", type=number, nargs="+", help="Pins to set HIGH")
    gpio_set_parser.add_argument("--off", type=number, nargs="+", help="Pins to set LOW")
    gpio_set_parser.add_argument("--high_z", action="store_true", help="Put ON pins to high Z state instead")
    gpio_subparsers.add_parser("get", help="Get GPIO pins")

    i2c_parser = subparsers.add_parser("i2c", help="Control I2C")
    i2c_subparsers = i2c_parser.add_subparsers(dest="i2c_command", required=True)
    i2c_set_speed_parser = i2c_subparsers.add_parser("set_speed", help="Set I2C speed")
    i2c_set_speed_parser.add_argument("speed", type=number, help="Speed in Hz")
    i2c_set_address_parser = i2c_subparsers.add_parser("set_address", help="Set I2C address")
    i2c_set_address_parser.add_argument("address", type=number, help="I2C address")
    i2c_set_timeout_parser = i2c_subparsers.add_parser("set_timeout", help="Set I2C timeout")
    i2c_set_timeout_parser.add_argument("timeout", type=number, help="I2C timeout")
    i2c_read_subparser = i2c_subparsers.add_parser("read", help="Read from I2C")
    i2c_read_subparser.add_argument("length", type=number, help="Length in bytes to read")
    i2c_write_subparser = i2c_subparsers.add_parser("write", help="Write to I2C")
    i2c_write_subparser.add_argument("data", type=number, nargs="+", help="Data to write")
    i2c_subparsers.add_parser("scan", help="Scan I2C addresses")

    spi_parser = subparsers.add_parser("spi", help="Control raw SPI")
    spi_subparsers = spi_parser.add_subparsers(dest="spi_command", required=True)
    spi_set_speed_parser = spi_subparsers.add_parser("set_speed", help="Set SPI speed")
    spi_set_speed_parser.add_argument("speed", type=number, help="Speed in Hz")
    spi_subparsers.add_parser("cs_select", help="Activate CS")
    spi_subparsers.add_parser("cs_deselect", help="Deactivate CS")
    spi_read_subparser = spi_subparsers.add_parser("read", help="Read from SPI")
    spi_read_subparser.add_argument("length", type=number, help="Length in bytes to read")
    spi_write_subparser = spi_subparsers.add_parser("write", help="Write to SPI")
    spi_write_subparser.add_argument("data", type=number, nargs="+", help="Data to write")

    flash_parser = subparsers.add_parser("flash", help="Control SPI flash")
    flash_subparsers = flash_parser.add_subparsers(dest="flash_command", required=True)
    flash_subparsers.add_parser("read_status", help="Read SPI flash status")
    flash_read_subparser = flash_subparsers.add_parser("read", help="Read from SPI flash")
    flash_read_subparser.add_argument("address", type=number, help="Address to read from")
    flash_read_subparser.add_argument("length", type=number, help="Length in bytes to read")
    flash_sector_erase_subparser = flash_subparsers.add_parser("sector_erase", help="Erase flash sector")
    flash_sector_erase_subparser.add_argument("address", type=number, help="Address of the sector to erase")
    flash_subparsers.add_parser("chip_erase", help="Erase flash")
    flash_program_subparser = flash_subparsers.add_parser("program", help="Program flash page")
    flash_program_subparser.add_argument("address", type=number, help="Address to write to")
    flash_program_subparser.add_argument("data", type=number, nargs="+", help="Data to write")
    flash_subparsers.add_parser("program_random", help="Program flash with 1MB of random data")

    args = parser.parse_args()

    with SerialCommunicator() as communicator:
        if args.command == "enter_bootsel":
            communicator.execute(enter_bootsel())

        if args.command == "gpio":
            if args.gpio_command == "set":
                pin_mask, pin_values = make_gpio_masks(args.on, args.off)

                if pin_mask == 0:
                    print("No pins specified.")
                    return

                if args.high_z:
                    communicator.execute(gpio_set_high_z(pin_mask, pin_values))
                else:
                    communicator.execute(gpio_set(pin_mask, pin_values))

            if args.gpio_command == "get":
                pin_state = communicator.execute(gpio_get())
                print(bin(pin_state))

        if args.command == "i2c":
            if args.i2c_command == "set_speed":
                communicator.execute(i2c_set_speed(args.speed))
            if args.i2c_command == "set_address":
                communicator.execute(i2c_set_address(args.address))
            if args.i2c_command == "set_timeout":
                communicator.execute(i2c_set_timeout(args.timeout))
            if args.i2c_command == "read":
                data = communicator.execute(i2c_read(args.length))
                print(data)
            if args.i2c_command == "write":
                communicator.execute(i2c_write(bytes(args.data)))
            if args.i2c_command == "scan":
                print("I2C Bus Scan")
                print("   0 1 2 3 4 5 6 7 8 9 A B C D E F", end="")
                communicator.execute(i2c_set_timeout(5000))
                for addr in range(1 << 7):
                    if addr % 16 == 0:
                        print(f"\n{addr:02x} ", end="")
                    communicator.execute(i2c_set_address(addr))
                    result = communicator.execute(i2c_check_ack())
                    print(f"{'@' if result else '.'} ", end="")
                print()

        if args.command == "spi":
            if args.spi_command == "set_speed":
                communicator.execute(spi_set_speed(args.speed))
            if args.spi_command == "cs_select":
                communicator.execute(spi_cs_select())
            if args.spi_command == "cs_deselect":
                communicator.execute(spi_cs_deselect())
            if args.spi_command == "read":
                data = communicator.execute(spi_read(args.length))
                print(data)
            if args.spi_command == "write":
                communicator.execute(spi_write(bytes(args.data)))

        if args.command == "flash":
            if args.flash_command == "read_status":
                status = communicator.execute(flash_read_status())
                print(status)
            if args.flash_command == "read":
                data = communicator.execute(flash_read(args.address, args.length))
                print(data)
            if args.flash_command == "sector_erase":
                communicator.execute(flash_sector_erase(args.address))
            if args.flash_command == "chip_erase":
                communicator.execute(flash_chip_erase())
            if args.flash_command == "program":
                communicator.execute(flash_program(args.address, bytes(args.data)))
            if args.flash_command == "program_random":
                start = time.perf_counter()
                chunk = 1024
                size = 1024**2
                for i in range(size // chunk):
                    address = i * chunk
                    communicator.execute(flash_program(address, randbytes(chunk)))
                end = time.perf_counter()
                elapsed = end - start
                print(f"Took {elapsed:.3f} s")


if __name__ == "__main__":
    main()
