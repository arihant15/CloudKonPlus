# CloudKonPlus
CloudKonPlus

## SOFTWARE REQUIREMENTS -------------------

1. Google protocol buffers c++ binding, VERSION 2.4.1
		http://code.google.com/p/protobuf/downloads/list
		(or)
		cd to CloudKonPlus/src/protobuf-2.4.1
		
2. Google protocol buffers c binding, VERSION 0.15 or later
		http://code.google.com/p/protobuf-c/downloads/list
		(or)
		cd to CloudKonPlus/src/protobuf-c-0.15
		

3. Install the below:
 ```sh
	sudo apt-get install gcc
	sudo apt-get install G++
	sudo apt-get install openjdk-7-jre
	sudo apt-get install libcurl3
	sudo apt-get install libcurlpp-dev
	sudo apt-get install libcurl-ocaml libcurl-ocaml-dev
	sudo apt-get install autoconf
	sudo apt-get install libtool
```

4. Install RestClient-CPP
```sh
	cd to CloudKonPlus/src/restclient-cpp-master
	libtoolize
	aclocal
	autoheader
	./autogen.sh
	./configure
	sudo make install
```
5. Install FUSE
```sh
	cd to CloudKonPlus/src/fuse-2.9.3
	./configure
	make
	make install
	modprobe fuse
```
6. Install Kafka and Zookeeper,

```sh
	http://czcodezone.blogspot.com/2014/11/setup-kafka-in-single-machine-running.html
```

## COMPILE

Assuming that you installed them to default directory, possibly that is, /usr/local
```sh
1. vim/gedit ~/.bashrc
```
2. append the following lines to the end,

```sh
	export USER_LIB=/usr/local/lib/:$USER_LIB
	export USER_INCLUDE=/usr/local/include/:$USER_INCLUDE
	export LD_LIBRARY_PATH=/usr/local/lib/:/CloudKonPlus/src/ffsnet/:/CloudKonPlus/src/udt/src:$LD_LIBRARY_PATH
```
3. source ~/.bashrc

4. Increase “Open Files Limit”: 
	https://rtcamp.com/tutorials/linux/increase-open-files-limit/

5. Increase Local Port Range with net.ipv4.ip_local_port_range Kernel Parameter:
	http://www.cyberciti.biz/tips/linux-increase-outgoing-network-sockets-range.html

6. ZHT/FaBRiQ:
```sh
	cd to CloudKonPlus/src/zht
	make clean
	make
```
7. UDT:
```sh
	cd to CloudKonPlus/src/udt
	make clean
	make
```
8. FFSNET:
```sh
	cd to CloudKonPlus/src/ffsnet
	make clean
	make
```
9. FusionFS:
```sh
	cd to CloudKonPlus/src/fusionfs
	make clean
	make
```
10. Monitoring Client:
```sh
	cd to CloudKonPlus/src/monitoring-client
	make clean
	make
```
11. CloudKonPlus:
```sh
	cd to CloudKonPlus/src/cloudkonplus
	make clean
	make
```
12. Word Count:

```sh
	cd to CloudKonPlus/src/wordcount
	make clean
	make
```
### CONF 
```sh
1. cd to CloudKonPlus/conf
2. Update "neighbor.conf" with IP and Port Number(50000)
```
###RUN ZHT SERVER
```sh
1. cd to CloudKonPlus/script
2. run ./start_service.sh
```
###RUN Monitoring Server
```sh
	1. cd to CloudKonPlus/kafkaCode/my-app
	2. Follow ReamMe
```
###RUN CloudKonPlus Worker
```sh
	1. cd to CloudKonPlus/script
	2. run ./start.sh
```

###RUN CloudKonPlus Client
```sh
	1. cd to CloudKonPlus/src/cloudkonplus
	2. $ ./ckp_client -z ../../conf/zht.conf -n ../../conf/neighbor.conf -w ../../TestCase/Throughput/sleep_0_10k.txt
```

###RUN WordCount Worker
```sh
	1. cd to CloudKonPlus/script
	2. run ./start_wc.sh
```
###RUN WordCount Client
```sh
1. cd to CloudKonPlus/src/wordcount
2. $ ./wc_client -z ../../conf/zht.conf -n ../../conf/neighbor.conf -w CloudKonPlus/TestCase/wordcount/5mb.txt
```
###RUN Stop Service
```sh
1. cd to CloudKonPlus/script
2. run ./stop.sh
3. run ./stop_service.sh
```
