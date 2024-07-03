qemu-system-x86_64 \
-accel hvf \
-cdrom Override.iso \
-m 2048 \
-net nic,model=virtio \
-net user,hostfwd=tcp::4242-:4242 \
