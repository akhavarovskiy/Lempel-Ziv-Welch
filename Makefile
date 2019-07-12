######################################
#
# Makefile: for building lzwLib.a
#
# source files are compiled and a library file created 
# which can be searched for static linking with 
# the instructor's testbed.
#
# You may need to change some of the following targets, 
# dependencies and productions if your source structure 
# deviates from my template structure.  Note the "clean"
# target at the end for removing all derived files. Both
# it and the "lzwLib.a" targets will be needed by the
# instructor's testbed build.
#

INCLUDE=include/
OBJ=obj/
SRC=src/
LIB=lib/

# do NOT modify these C flags! (you shouldn't want to!)
CFLAGS=-c -std=c99 -Wall -pedantic -I$(INCLUDE) -g

$(LIB)liblzw.a: $(OBJ)lzwEncode.o $(OBJ)lzwDecode.o $(OBJ)sequence.o $(OBJ)dict.o $(OBJ)bitStream.o
	ar cru $(LIB)liblzw.a $(OBJ)lzwEncode.o $(OBJ)lzwDecode.o $(OBJ)sequence.o $(OBJ)dict.o $(OBJ)bitStream.o
	ranlib $(LIB)liblzw.a

$(OBJ)lzwEncode.o: $(SRC)lzwEncode.c $(INCLUDE)lzw.h $(INCLUDE)bitStream.h $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS)  $(SRC)lzwEncode.c -o obj/lzwEncode.o

$(OBJ)lzwDecode.o: $(SRC)lzwDecode.c $(INCLUDE)lzw.h $(INCLUDE)bitStream.h $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS)  $(SRC)lzwDecode.c -o obj/lzwDecode.o

$(OBJ)bitStream.o: $(SRC)bitStream.c $(INCLUDE)bitStream.h
	gcc $(CFLAGS)  $(SRC)bitStream.c -o obj/bitStream.o

$(OBJ)dict.o:      $(SRC)dict.c $(INCLUDE)dict.h $(INCLUDE)sequence.h
	gcc $(CFLAGS)  $(SRC)dict.c      -o obj/dict.o

$(OBJ)sequence.o:  $(SRC)sequence.c $(INCLUDE)sequence.h
	gcc $(CFLAGS)  $(SRC)sequence.c  -o obj/sequence.o

clean:
	rm -f $(LIB)liblzw.a $(OBJ)lzwEncode.o $(OBJ)lzwDecode.o $(OBJ)bitStream.o $(OBJ)dict.o $(OBJ)sequence.o