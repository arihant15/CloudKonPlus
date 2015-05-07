#usage: make [-e prefix=/home/dongfang/software]

ifndef prefix
	#prefix = $(HOME)/software
	prefix = /usr/local
endif

all: ffsnet_test_c libffsnet_bridger.so libffsnet.so ffsnetd

ffsnet_test_c: ffsnet_test_c.c libffsnet_bridger.so
	gcc ffsnet_test_c.c -L. -lffsnet_bridger -o ffsnet_test_c

libffsnet_bridger.so: ffsnet_bridger.cpp libffsnet.so
	g++ -fPIC ffsnet_bridger.cpp --shared -o libffsnet_bridger.so -L. -lffsnet
	cp $@ $(prefix)"/lib/"$@
	
libffsnet.so: ffsnet.cpp ffsnet.h
	g++ ffsnet.cpp -fPIC --shared -o libffsnet.so -I../udt -L../udt -ludt -lstdc++ -lpthread
	cp $@ $(prefix)"/lib/"$@
	
ffsnetd: ffsnetd.cpp	
	g++ ffsnetd.cpp -o ffsnetd -I../udt -L../udt -ludt -lstdc++ -lpthread
	cp $@ $(prefix)"/bin/"$@
	
clean:
	rm ffsnet_test_c ffsnetd libffsnet_bridger.so libffsnet.so
