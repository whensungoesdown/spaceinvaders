#!/bin/bash

riscv32-unknown-linux-gnu-objcopy --dump-section .text=space.bin spaceinvaders

truncate -s 2k space.bin

hexdump space.bin
