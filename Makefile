CC = cc

# For optimized output
#CFLAGS = -Wall -Wextra -O3

# For debugging
CFLAGS = -O3
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build

# List of source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files derived from source files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Binary name
TARGET = bounce

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

install: $(TARGET)
	cp -fv $(TARGET) /usr/local/bin

clean:
	rm -rf $(BUILD_DIR) $(TARGET)


.PHONY: all clean install

