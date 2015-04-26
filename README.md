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

1. $ gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c ckp_worker.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
2. $ gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o ckp_worker ckp_worker.o lock_guard.o cpp_zhtclient.o meta.pb-c.o lru_cache.o meta.pb.o zpack.pb.o novoht.o bigdata_transfer.o Const.o ConfHandler.o ConfEntry.o ProxyStubFactory.o proxy_stub.o ip_proxy_stub.o mq_proxy_stub.o ipc_plus.o tcp_proxy_stub.o udp_proxy_stub.o ZHTUtil.o Env.o Util.o HTWorker.o StrTokenizer.o TSafeQueue.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
3. $ ./ckp_worker -z zht.conf -n neighbor.conf -t #numThreads

-------------------RUN CloudKon Client-------------------

1. $ gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c ckp_client.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
2. $ gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o ckp_client ckp_client.o lock_guard.o cpp_zhtclient.o meta.pb-c.o lru_cache.o meta.pb.o zpack.pb.o novoht.o bigdata_transfer.o Const.o ConfHandler.o ConfEntry.o ProxyStubFactory.o proxy_stub.o ip_proxy_stub.o mq_proxy_stub.o ipc_plus.o tcp_proxy_stub.o udp_proxy_stub.o ZHTUtil.o Env.o Util.o HTWorker.o StrTokenizer.o TSafeQueue.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
3. $ ./ckp_client -z zht.conf -n neighbor.conf -w TestCase.txt