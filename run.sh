#!/bin/env bash

# Function to run QEMU with the appropriate accelerator
run_qemu() {
  qemu-system-x86_64 \
  -accel $1 \
  -cdrom Override.iso \
  -m 2048 \
  -net nic,model=virtio \
  -net user,hostfwd=tcp::4242-:4242
}

# Detect the operating system
OS=$(uname -s)

case $OS in
  Darwin)
    # macOS
    ACCEL="hvf"
    ;;
  Linux)
    # Linux
    ACCEL="kvm"
    ;;
  *)
    echo "Unsupported operating system: $OS"
    exit 1
    ;;
esac

run_qemu $ACCEL
