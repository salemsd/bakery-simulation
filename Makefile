# Bakery simulation
.PHONY: all dist docs clean

# flags
CC =gcc
CFLAGS =-std=c99 -Wall -g
CPPFLAGS =-Iinclude
LDFLAGS =-lm

# executable
BIN_DIR = bin
TARGET = $(BIN_DIR)/bakery

# chemins
vpath %.c src/
vpath %.h include/

# .o
OBJ_DIR = obj/
OBJ = $(patsubst ./src/%.c,%.o,$(wildcard ./src/*.c))


all: $(TARGET) 

$(TARGET): $(addprefix $(OBJ_DIR),$(OBJ))
	mkdir -p $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "\nUse \"make dist\" to zip the project files\n"
	@echo "Use \"make clean\" to remove object and binary files\n"
	@echo "Use \"make help\" anytime to see this message again\n"
	@echo "\nHow to run: ./bin/bakery\n"

$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@	
	
dist:
	mkdir -p $@
	zip -ru ./dist/TP5_Salem.zip src include Makefile reponse.txt

help:
	@echo "\nUse \"make\" to compile the project\n"
	@echo "Use \"make dist\" to zip the project files\n"
	@echo "Use \"make clean\" to remove object and binary files\n"
	@echo "Use \"make help\" anytime to see this message again\n"
	@echo "\nHow to run: ./bin/bakery\n"

clean:
	$(RM) -r dist/ $(OBJ_DIR) $(BIN_DIR)/ *.o bakery