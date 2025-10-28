#!/usr/bin/env python3

import argparse
from serial_comm import CdcGpioController as UsbGpioController


def make_gpio_masks(gpios_on, gpios_off):
    pin_mask = 0
    pin_values = 0

    if gpios_on:
        for pin in gpios_on:
            pin_mask |= (1 << pin)
            pin_values |= (1 << pin)

    if gpios_off:
        for pin in gpios_off:
            pin_mask |= (1 << pin)
            pin_values &= ~(1 << pin)

    return pin_mask, pin_values


def main():
    parser = argparse.ArgumentParser(description="Control GPIOs via USB device.")
    subparsers = parser.add_subparsers(dest='command', required=True)

    gpio_parser = subparsers.add_parser('gpio', help='Control GPIO pins')
    gpio_subparsers = gpio_parser.add_subparsers(dest='gpio_command', required=True)
    gpio_set_parser = gpio_subparsers.add_parser('set', help='Set GPIO pins')
    gpio_set_parser.add_argument('--on', type=int, nargs='+', help='Pins to set HIGH')
    gpio_set_parser.add_argument('--off', type=int, nargs='+', help='Pins to set LOW')
    gpio_get_parser = gpio_subparsers.add_parser('get', help='Get GPIO pins')

    args = parser.parse_args()

    if args.command == 'gpio' and args.gpio_command == "set":
        controller = UsbGpioController()

        pin_mask, pin_values = make_gpio_masks(args.on, args.off)

        if pin_mask == 0:
            print("No pins specified.")
            return

        controller.set_pins(pin_mask, pin_values)

    if args.command == 'gpio' and args.gpio_command == "get":
        controller = UsbGpioController()
        pin_state = controller.get_pins()
        print(pin_state)


if __name__ == "__main__":
    main()
