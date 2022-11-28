CC = g++
FLAGS = -Wall -Wextra -Wconversion -static -DONLINE_JUDGE -Wl,--stack=268435456 -O2 -std=c++20
CFLAGS = -Wall -Wextra -Wconversion -static -DONLINE_JUDGE -Wl,--stack=268435456 -O2 -std=c++20 -c
OBJ = digestFragment.o
EXE = digestFragment

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(FLAGS) -o $(EXE) $(OBJ)

$(OBJ):
	$(CC) $(CFLAGS) $(EXE).cpp

clean:
	rm -f $(OBJ)
