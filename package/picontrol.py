#!/usr/bin/env python3

import argparse
import struct

import defs
from gpio_commands import gpio_get, gpio_set
from i2c_commands import i2c_check_ack, i2c_read, i2c_set_address
from serial_comm import SerialCommunicator


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


def main():
    parser = argparse.ArgumentParser(description="Control GPIOs via USB device.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    gpio_parser = subparsers.add_parser("gpio", help="Control GPIO pins")
    gpio_subparsers = gpio_parser.add_subparsers(dest="gpio_command", required=True)
    gpio_set_parser = gpio_subparsers.add_parser("set", help="Set GPIO pins")
    gpio_set_parser.add_argument("--on", type=int, nargs="+", help="Pins to set HIGH")
    gpio_set_parser.add_argument("--off", type=int, nargs="+", help="Pins to set LOW")
    gpio_get_parser = gpio_subparsers.add_parser("get", help="Get GPIO pins")

    i2c_parser = subparsers.add_parser("i2c", help="Control I2C")
    i2c_subparsers = i2c_parser.add_subparsers(dest="i2c_command", required=True)
    i2c_scan_parser = i2c_subparsers.add_parser("scan", help="Scan I2C addresses")

    args = parser.parse_args()

    with SerialCommunicator() as communicator:
        if args.command == "gpio" and args.gpio_command == "set":
            pin_mask, pin_values = make_gpio_masks(args.on, args.off)

            if pin_mask == 0:
                print("No pins specified.")
                return

            communicator.execute(gpio_set(pin_mask, pin_values))

        if args.command == "gpio" and args.gpio_command == "get":
            pin_state = communicator.execute(gpio_get())
            print(bin(pin_state))

        if args.command == "i2c" and args.i2c_command == "scan":
            print("I2C Bus Scan")
            print("   0 1 2 3 4 5 6 7 8 9 A B C D E F", end="")

            for addr in range(1 << 7):
                if addr % 16 == 0:
                    print(f"\n{addr:02x} ", end="")
                communicator.execute(i2c_set_address(addr))
                result = communicator.execute(i2c_check_ack())
                print(f"{'@' if result else '.'} ", end="")
            print()


if __name__ == "__main__":
    main()
