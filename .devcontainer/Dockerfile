FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install base tools
RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y \
    build-essential \
    gcc-multilib \
    g++-multilib \
    libc6-dev-i386 \
    libc6:i386 \
    libc6-dbg:i386 \
    libstdc++6:i386 \
    valgrind \
    valgrind-dbg \
    gdb \
    nasm \
    && rm -rf /var/lib/apt/lists/*
# Create vscode user
RUN useradd -ms /bin/bash vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN printf '%s\n' \
'echo "============================="' \
'echo "  Made by Marae Bshoty"' \
'echo "  Feel free to contact me at 052-3498065"' \
'echo "  For a private/group lesson"' \
'echo "  Write clear to remove this message"' \
'echo "============================="' \
>> /home/vscode/.bashrc

USER vscode
WORKDIR /workspace