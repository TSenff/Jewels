CC=gcc
CFLAG= -Wall -I -std=c99 -DDEBUGGER 
ALLEGRO=-lallegro_image  -lallegro_primitives  -lallegro_dialog  -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec  -lallegro -lm

LFLAG= $(ALLEGRO)



all : game.o auxiliar.o board.o interface.o
	$(CC) $(CFLAG) game.o auxiliar.o board.o interface.o -o jewels $(LFLAG)

game.o : game.c auxiliar.h board.h interface.h
	$(CC) -c   $(CFLAG) game.c

auxiliar.o: auxiliar.c
	$(CC) -c $(CFLAG) auxiliar.c -lm

board.o : board.c
	$(CC) -c $(CFLAG) board.c

interface.o : interface.c interface.h 
	$(CC) -c $(CFLAG) interface.c -lm

clean : 
	rm -f *.o

purge : clean
	rm -f jewels