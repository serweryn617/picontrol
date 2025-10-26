FROM ubuntu:22.04

RUN apt update && apt install -y \
    git \
    cmake \
    ninja-build \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    build-essential \
    python3 \
    && apt clean
