CC=gcc
CFLAG= -Wall -I -std=c99
ALLEGRO=-lallegro_image  -lallegro_primitives  -lallegro_dialog  -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec  -lallegro

LFLAG= $(ALLEGRO)

all : game.o auxiliar.o board.o
	$(CC) $(CFLAG)  game.o auxiliar.o board.o -o jewels $(LFLAG)

teste : main.c board.o auxiliar.o
	$(CC) $(CFLAG)  main.c board.o auxiliar.o -o teste $(LFLAG)


game.o : game.c auxiliar.h board.h
	$(CC) -c $(CFLAG) game.c

auxiliar.o: auxiliar.c
	$(CC) -c $(CFLAG) auxiliar.c

board.o : board.c
	$(CC) -c $(CFLAG) board.c


clean : 
	rm -f *.o

purge : clean
	rm -f jewels teste