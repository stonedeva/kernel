#!/bin/bash
qemu-system-i386 \
  -m 512 \
  -cdrom kernel.iso \
  -boot d \
  -serial stdio \
  -drive file=fs/empty.img,format=raw,if=ide
