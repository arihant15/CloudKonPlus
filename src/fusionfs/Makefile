#usage: make [-e prefix=/home/dongfang/software]
ifndef prefix
	#prefix = $(HOME)/software
	prefix = /usr/local
endif
SOFTWARE=$(HOME)/software
INC=-I$(SOFTWARE)/include -D_FILE_OFFSET_BITS=64
LIB=-L$(SOFTWARE)/lib 

fusionfs : fusionfs.o util.o log.o
	g++ -g -Wall -o $@ $^ ${LIB} -lfuse -lzht -lprotobuf -lprotobuf-c -L../ffsnet -lffsnet_bridger -pthread
	cp $@ $(prefix)/bin/$@

fusionfs.o : fusionfs.c log.h params.h util.h
	gcc -g -Wall ${INC} -c fusionfs.c 

log.o : log.c log.h params.h
	gcc -g -Wall ${INC} -c log.c
	
util.o : util.c log.h params.h
	gcc -g -Wall ${INC} -c util.c

.PHONY: clean
clean:
	rm -f fusionfs *.o
