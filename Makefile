CC  = gcc
ASM = nasm

BIN_DIR = .bin
OBJ_DIR = .obj
ISO_DIR = iso
ISO     = quneos.iso
KERNEL  = $(BIN_DIR)/kernel.bin

CFLAGS   = -m32 -ffreestanding -fno-pie -fno-stack-protector -fno-builtin -O2 -Wall -Wextra -I. -Iinclude
ASMFLAGS = -f elf32
LDFLAGS  = -m32 -nostdlib -no-pie -T linker.ld

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

C_SRCS   = $(call rwildcard,.,*.c)
ASM_SRCS = $(call rwildcard,.,*.asm)

OBJ_C   = $(patsubst ./%.c, $(OBJ_DIR)/%.o, $(C_SRCS))
OBJ_ASM = $(patsubst ./%.asm, $(OBJ_DIR)/%_asm.o, $(ASM_SRCS))
OBJECTS = $(OBJ_ASM) $(OBJ_C)

all: $(BIN_DIR)/$(ISO)

run: all
	@qemu-system-i386 -cdrom $(BIN_DIR)/$(ISO) -boot d

$(BIN_DIR)/$(ISO): $(KERNEL) grub.cfg
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	@cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	@grub-mkrescue -o $@ $(ISO_DIR) >/dev/null 2>&1
	@rm -rf $(ISO_DIR)
	@printf "\033[1;38;5;34mISO is ready: %s\n" "$@"

$(KERNEL): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking Kernel..."
	@$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%_asm.o: %.asm | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "ASM $<"
	@$(ASM) $(ASMFLAGS) $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) $(ISO_DIR) *.iso
	@echo "Cleaned!"

.PHONY: all run clean
