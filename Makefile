BUILD_DIR := build
SRC_DIR := src

C_SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
ASM_SRC_FILES := $(shell find $(SRC_DIR) -name '*.asm')

C_OBJ_FILES := ${C_SRC_FILES:.c=.o}
ASM_OBJ_FILES := ${ASM_SRC_FILES:.asm=.o}

KERNEL := kernel.bin

# compilers
CC=gcc
CFLAGS=-m32 -T inc/ -std-gnu99 -fno-pie -ffreestanding -nostdlib -O2 -Wall -Wextra

ASM := nasm
ASM_FLAGS := -f elf32

KOBJS := $(wildcard build/*.o)

all:
	echo Source file are $(C_SRC_FILES)
	echo Object files are $(C_OBJ_FILES)

clean:
	rm $(BUILD_DIR)/*.o
	rm kernel.bin

$(KERNEL): $(C_OBJ_FILES) $(ASM_OBJ_FILES)
	ld -m elf_i386 -T link.ld -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $$(@D)/.
	$(ASM) $(ASM_FLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $$(@D)/.
	$(CC) $(CFLAGS) -o $@ $^
