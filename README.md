# Mastermind

## Requirements

To build the game, you need:

- `gcc` compiler
- `make`

## Build

From the root of the project, run:

```bash
mkdir build
make
build/mastermind
```

## Usage

```
Usage:
  mastermind [options]

Options:
  -h              Show this help and exit
  -m <0|1>        Game mode:
                    0 - PvE (player vs computer)
                    1 - EvP (computer vs player)
  -d <0|1>        Difficulty level:
                    0 - basic
                    1 - hard
  -l              Show scoreboard and exit

Examples:
  mastermind -m 0 -d 1     PvE, hard difficulty
  mastermind -m 1          EvP, basic difficulty
  mastermind -l            Scoreboard

Without arguments, the game starts in default mode:
  PvE, basic difficulty.
```
