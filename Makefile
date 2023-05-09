CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS =
SRC = main.c readMap.c adjListUtils.c routePlan.c visualize.c
OBJ = $(SRC:.c=.o)
DEP = readMap.h adjList.h routePlan.h visualize.h
LINKS = -lSDL2
INCDIR = -I/usr/include/SDL2

all: main

main: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LINKS) -o $@

%.o: %.c $(DEP)
	$(CC) $(CFLAGS) $(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJ) main

	

# CC  = gcc
# CFLAGS = -std=c99 -Wall -Werror
# EXE = draft
# LINKS = -lSDL2
# INCDIR = -I/usr/include/SDL2
# all: ${EXE}

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $< -o $@  $(LINKS) $(INCDIR)
# draft: draft.o
# 	$(CC) $(CCFLAGS) $^ -o $@ $(LINKS) $(INCDIR)
# clean:
# 	rm -f *.o ${EXE}
	
# main.c with readMap.c
# CC  = gcc
# CFLAGS = -std=c99 -Wall -Werror
# EXE = main
# LINKS = -lSDL2
# INCDIR = -I/usr/include/SDL2
# all: ${EXE}

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $< -o $@  $(LINKS) $(INCDIR)
# main: main.o readMap.o 
# 	$(CC) $(CCFLAGS) $^ -o $@ $(LINKS) $(INCDIR)

# clean:
# 	rm -f *.o ${EXE}
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

# CC  = gcc
# CFLAGS = -std=c99 -Wall -Werror
# EXE = initSDL
# LINKS = -lSDL2
# INCDIR = -I/usr/include/SDL2
# all: ${EXE}
# %.o: %.c
# 	$(CC) -c $(CFLAGS) $< -o $@  $(LINKS) $(INCDIR)
# initSDL: initSDL.o
# 	$(CC) $(CCFLAGS) $^ -o $@ $(LINKS) $(INCDIR)
# clean:
# 	rm -f *.o ${EXE}