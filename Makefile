CC       := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS := -Isrc -Isrc/game_api
LDFLAGS  :=
LDLIBS   := -lm

SRC_DIR   := src
GAME_DIR  := src/game_api
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c) \
        $(wildcard $(GAME_DIR)/*.c)


OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET := $(BUILD_DIR)/mastermind

.PHONY: all clean run rebuild debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

run: all
	./$(TARGET)

debug: CFLAGS += -DDEBUG -O0
debug: rebuild

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)