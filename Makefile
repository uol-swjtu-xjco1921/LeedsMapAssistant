EXE = readMap
SRC = readMap.c
LINKS = -lSDL2
INCDIR = -I/usr/include/SDL2
all:
	gcc -o $(EXE) $(SRC) $(LINKS) $(INCDIR)
	
clean:
	rm -f $(EXE)
