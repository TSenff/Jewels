CC=gcc
CFLAG= -Wall -I -std=c99
ALLEGRO=-lallegro_image  -lallegro_primitives  -lallegro_dialog  -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec  -lallegro -lm

LFLAG= $(ALLEGRO)

all : game.o auxiliar.o board.o
	$(CC) $(CFLAG)  game.o auxiliar.o board.o -o jewels $(LFLAG)

#Outros metodos de compilar

# Roda o jogo sem interface grafica 
terminal : terminal.o board.o auxiliar.o
	$(CC) $(CFLAG)  terminal.o board.o auxiliar.o -o terminal $(LFLAG)

# Area para teste, remover depois 
teste : main.o board.o auxiliar.o
	$(CC) $(CFLAG)  main.0 board.o auxiliar.o -o teste $(LFLAG) 


# Objetos 
terminal.o : terminal.c board.h auxiliar.h
	$(CC) -c $(CFLAG)  terminal.c -o terminal.o 

	

game.o : game.c auxiliar.h board.h
	$(CC) -c $(CFLAG) game.c

auxiliar.o: auxiliar.c
	$(CC) -c $(CFLAG) auxiliar.c -lm

board.o : board.c
	$(CC) -c $(CFLAG) board.c


clean : 
	rm -f *.o

purge : clean
	rm -f jewels teste terminal