

#COMPILER VERSION
CC = gcc
#COMPILER FLAGS
CCFLAGS = -Wall -pedantic
#MAKEFILE OPTIONS
APP_NAME = przetwarzanieObrazow
EX = .c
SRC = src/
OBJ = obj/
INC = inc/

#Creating executable program

output: $(SRC)przetwarzanieObrazowV2.c $(OBJ)filtry.o $(OBJ)obslugaPlikow.o $(OBJ)opcje.o     
	$(CC) $(SRC)przetwarzanieObrazowV2.c $(CCFLAGS) $(OBJ)filtry.o $(OBJ)obslugaPlikow.o $(OBJ)opcje.o -o $(APP_NAME) 

obj/filtry.o: $(SRC)filtry.c $(INC)filtry.h     
	$(CC) -c -o $(OBJ)filtry.o $(SRC)filtry.c  

obj/opcje.o: $(SRC)opcje.c $(INC)opcje.h
	$(CC) -c -o $(OBJ)opcje.o $(SRC)opcje.c

obj/obslugaPlikow.o: $(SRC)obslugaPlikow.c $(INC)obslugaPlikow.h     
	$(CC) -c -o $(OBJ)obslugaPlikow.o $(SRC)obslugaPlikow.c  

#Cleaning all .o stuff
clean: 
	rm $(OBJ)*.o $(APP_NAME)
