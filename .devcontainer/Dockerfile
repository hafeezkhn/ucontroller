FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu
ENV DEBIAN_FRONTEND=noninteractive

RUN sudo apt-get update && sudo apt-get install -y \
    build-essential \
    binutils-common \
    binutils \
    make \
    gdb-multiarch \
    git-core \
    wget \
    qemu-system-arm \
    gcc-arm-none-eabi \
    gdb-multiarch \
    libnewlib-arm-none-eabi \
    openocd \
    python3 \
    python3-dev \
    python3-pip \
    && sudo rm -rf /var/lib/apt/lists/*

ENV TERM xterm

