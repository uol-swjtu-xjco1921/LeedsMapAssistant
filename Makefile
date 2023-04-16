CC  = gcc
CFLAGS = -std=c99 -Wall -Werror
EXE = main
LINKS = -lSDL2
INCDIR = -I/usr/include/SDL2
all: ${EXE}

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@  $(LINKS) $(INCDIR)
main: main.o readMap.o 
	$(CC) $(CCFLAGS) $^ -o $@ $(LINKS) $(INCDIR)

clean:
	rm -f *.o ${EXE}
# CC  = gcc
# CFLAGS = -std=c99 -Wall -Werror
# EXE = readMap
# LINKS = -lSDL2
# INCDIR = -I/usr/include/SDL2
# all: ${EXE}
# %.o: %.c
# 	$(CC) -c $(CFLAGS) $< -o $@  $(LINKS) $(INCDIR)
# readMap: readMap.o
# 	$(CC) $(CCFLAGS) $^ -o $@ $(LINKS) $(INCDIR)
# clean:
# 	rm -f *.o ${EXE}