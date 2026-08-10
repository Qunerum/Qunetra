CC = gcc
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -O2 -Wall -Wextra \
         -mno-sse -mno-sse2 -mno-mmx -mno-80387 -mpreferred-stack-boundary=2
NASM = nasm
LD = ld

BUILD_DIR = bin
OBJ_DIR = $(BUILD_DIR)/objs

C_SOURCES = $(wildcard drivers/*.c kernel/*.c cpu/*.c)
C_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(C_SOURCES))

KERNEL_ENTRY_OBJ = $(OBJ_DIR)/boot/kernel_entry.o

all: $(BUILD_DIR)/os-image.bin

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

$(BUILD_DIR)/boot.bin: boot/boot.asm | $(BUILD_DIR)
	$(NASM) -f bin $< -o $@

$(KERNEL_ENTRY_OBJ): boot/kernel_entry.asm | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

$(OBJ_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.bin: $(KERNEL_ENTRY_OBJ) $(C_OBJS) | $(BUILD_DIR)
	$(LD) -m elf_i386 -T linker.ld -o $@ --oformat binary $^

$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin | $(BUILD_DIR)
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $@
	truncate -s 65536 $@

run: $(BUILD_DIR)/os-image.bin
	qemu-system-i386 -vga std -usb -device -drive format=raw,file=$(BUILD_DIR)/os-image.bin

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
