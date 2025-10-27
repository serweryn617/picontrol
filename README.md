## Building firmware

Prerequisites:
 - `docker`
 - `cook` (`pipx install cook-builder`)

To build firmware run `cook`.

Pico firmware (`.uf2`) will land in `output` directory.

## USB endpoints performance on Pico

Testing bulk endpoint (64B packet)
Sent 1024.00 KiB in 2.6477 s
Throughput: 386.75 KiB/s (3168.28 Kbit/s)

Testing bulk endpoint (512kB packet)
Sent 1024.00 KiB in 1.4602 s
Throughput: 701.25 KiB/s (5744.66 Kbit/s)

Testing interrupt endpoint
Sent 1024.00 KiB in 16.3834 s
Throughput: 62.50 KiB/s (512.02 Kbit/s)

Testing HID control endpoint
Sent 1024.00 KiB in 49.1510 s
Throughput: 20.83 KiB/s (170.67 Kbit/s)

Testing CDC (64B packet)
Sent 1024.00 KiB in 2.0640 s
Throughput: 496.13 KiB/s (4064.26 Kbit/s)

Testing CDC (512kB packet)
Sent 1024.00 KiB in 2.1377 s
Throughput: 479.02 KiB/s (3924.15 Kbit/s)