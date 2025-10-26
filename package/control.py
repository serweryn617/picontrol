#!/usr/bin/env python3
import argparse
from usb_comm import UsbGpioController

def main():
    parser = argparse.ArgumentParser(description="Control GPIOs via USB device.")
    subparsers = parser.add_subparsers(dest='command', required=True)

    # GPIO subparser
    gpio_parser = subparsers.add_parser('gpio', help='Control GPIO pins')
    gpio_parser.add_argument('--on', type=int, nargs='+', help='Pins to set HIGH')
    gpio_parser.add_argument('--off', type=int, nargs='+', help='Pins to set LOW')

    args = parser.parse_args()

    if args.command == 'gpio':
        controller = UsbGpioController()

        # Build mask and values
        pin_mask = 0
        pin_values = 0

        if args.on:
            for pin in args.on:
                pin_mask |= (1 << pin)
                pin_values |= (1 << pin)

        if args.off:
            for pin in args.off:
                pin_mask |= (1 << pin)
                pin_values &= ~(1 << pin)

        if pin_mask == 0:
            print("No pins specified.")
            return

        controller.set_pins(pin_mask, pin_values)

if __name__ == "__main__":
    main()
