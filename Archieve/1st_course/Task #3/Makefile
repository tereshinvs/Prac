SRC_PATH := src
INCLUDE_PATH := include
BIN_PATH := bin
OBJ_PATH := obj

GCC_PARAMS := -m32 -std=c99 -g -gstabs
NASM_PARAMS := -f elf32
ASM_OBJECTS := swap.o compare.o timestamp.o
ALL_OBJECTS := $(addprefix $(OBJ_PATH)/, heapsort.o shellsort.o $(ASM_OBJECTS))
ASM_OBJECTS := $(addprefix $(OBJ_PATH)/, $(ASM_OBJECTS))


all: $(BIN_PATH)/ $(OBJ_PATH)/ shell_inc_sort shell_dec_sort heap_inc_sort heap_dec_sort


shell_inc_sort: $(SRC_PATH)/main.c $(ALL_OBJECTS)
	gcc $(GCC_PARAMS) -o $(BIN_PATH)/$@ $^ -I $(INCLUDE_PATH) -D INCREASING -D SHELL


shell_dec_sort: $(SRC_PATH)/main.c $(ALL_OBJECTS)
	gcc $(GCC_PARAMS) -o $(BIN_PATH)/$@ $^ -I $(INCLUDE_PATH) -D DECREASING -D SHELL


heap_inc_sort: $(SRC_PATH)/main.c $(ALL_OBJECTS)
	gcc $(GCC_PARAMS) -o $(BIN_PATH)/$@ $^ -I $(INCLUDE_PATH) -D INCREASING -D HEAP


heap_dec_sort: $(SRC_PATH)/main.c $(ALL_OBJECTS)
	gcc $(GCC_PARAMS) -o $(BIN_PATH)/$@ $^ -I $(INCLUDE_PATH) -D DECREASING -D HEAP


$(OBJ_PATH)/heapsort.o: $(SRC_PATH)/sorts/heapsort.c
	gcc $(GCC_PARAMS) -o $@ $< -c -I $(INCLUDE_PATH)


$(OBJ_PATH)/shellsort.o: $(SRC_PATH)/sorts/shellsort.c
	gcc $(GCC_PARAMS) -o $@ $< -c -I $(INCLUDE_PATH)


$(OBJ_PATH)/compare.o: $(SRC_PATH)/asm/compare.asm
	nasm $(NASM_PARAMS) -o $@ $<


$(OBJ_PATH)/swap.o: $(SRC_PATH)/asm/swap.asm
	nasm $(NASM_PARAMS) -o $@ $<


$(OBJ_PATH)/timestamp.o: $(SRC_PATH)/asm/timestamp.asm
	nasm $(NASM_PARAMS) -o $@ $<


$(BIN_PATH)/:
	mkdir -p $(BIN_PATH)


$(OBJ_PATH)/:
	mkdir -p $(OBJ_PATH)


clean:
	rm -rf $(BIN_PATH) $(OBJ_PATH)

