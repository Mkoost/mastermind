CC     = gcc
CFLAGS = -O2 -g -Igame_api
LDLIBS = -lm

BUILD = ../build

OBJS = $(BUILD)/main.o \
       $(BUILD)/CLI.o \
       $(BUILD)/game_solver.o \
       $(BUILD)/games.o \
       $(BUILD)/game_api.o \
       $(BUILD)/game_error_api.o \
       $(BUILD)/IO.o \
       $(BUILD)/ScoreList.o

vpath %.c game_api

mastermind: $(OBJS)
	$(CC) $(OBJS) -o $(BUILD)/$@ $(LDLIBS)


$(BUILD)/%.o: %.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

run: mastermind
	$(BUILD)/mastermind

clean:
	rm -rf $(BUILD)/

