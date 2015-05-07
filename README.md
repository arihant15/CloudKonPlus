# CloudKonPlus
CloudKonPlus

-------------------SOFTWARE REQUIREMENTS-------------------

Google protocol buffers c binding, VERSION 0.15 or later
		http://code.google.com/p/protobuf-c/downloads/list


Google protocol buffers c++ binding, VERSION 2.4.1
		http://code.google.com/p/protobuf/downloads/list


-------------------COMPILE AND INSTALL-------------------

FIRSTLY install Google protocol buffers c++ binding, 
AND THEN install Google protocol buffers c binding since the latter dependes on the former.

Assuming that you installed them to default directory, possibly that is, /usr/local

1. vim/gedit ~/.bashrc
2. append the following lines to the end,
	
	export USER_LIB=/usr/local/lib/:$USER_LIB

	export USER_INCLUDE=/usr/local/include/:$USER_INCLUDE

	export LD_LIBRARY_PATH=/usr/local/lib/:$LD_LIBRARY_PATH

3. source ~/.bashrc
4. cd to ZHT src directory
5. TO COMPILE ZHT/FaBRiQ, RUN
   make

-------------------RUN ZHT SERVER-------------------

./zhtserver -z zht.conf -n neighbor.conf, OR
./zhtserver -z zht.conf -n neighbor.conf -p 40000, to specify your own port that overrides PORT in zht.conf, OR
./zhtserver -z zht.conf -n neighbor.conf -p 40000 -f novoht.db, to specify your own disk file to which data is persisted.

-------------------RUN CloudKon Worker-------------------


-------------------RUN CloudKon Client-------------------

1. $ ./ckp_client -z ../../conf/zht.conf -n ../../conf/neighbor.conf -w ../../TestCase/Throughput/sleep_0_5k.txt
