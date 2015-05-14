cloudkonplus:
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c ckp_client.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c -std=gnu++11 
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o ckp_client ckp_client.o ../zht/lock_guard.o ../zht/cpp_zhtclient.o ../zht/meta.pb-c.o ../zht/lru_cache.o ../zht/meta.pb.o ../zht/zpack.pb.o ../zht/novoht.o ../zht/bigdata_transfer.o ../zht/Const.o ../zht/ConfHandler.o ../zht/ConfEntry.o ../zht/ProxyStubFactory.o ../zht/proxy_stub.o ../zht/ip_proxy_stub.o ../zht/mq_proxy_stub.o ../zht/ipc_plus.o ../zht/tcp_proxy_stub.o ../zht/udp_proxy_stub.o ../zht/ZHTUtil.o ../zht/Env.o ../zht/Util.o ../zht/HTWorker.o ../zht/StrTokenizer.o ../zht/TSafeQueue.o ../zht/ipFinder.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c -std=gnu++11
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c ckp_worker.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c -std=gnu++11 
	libtool --mode=link gcc -std=gnu++11 -g -I -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o ckp_worker ckp_worker.o ../monitoring-client/mclient.o ../zht/lock_guard.o ../zht/cpp_zhtclient.o ../zht/meta.pb-c.o ../zht/lru_cache.o ../zht/meta.pb.o ../zht/zpack.pb.o ../zht/novoht.o ../zht/bigdata_transfer.o ../zht/Const.o ../zht/ConfHandler.o ../zht/ConfEntry.o ../zht/ProxyStubFactory.o ../zht/proxy_stub.o ../zht/ip_proxy_stub.o ../zht/mq_proxy_stub.o ../zht/ipc_plus.o ../zht/tcp_proxy_stub.o ../zht/udp_proxy_stub.o ../zht/ZHTUtil.o ../zht/Env.o ../zht/Util.o ../zht/HTWorker.o ../zht/StrTokenizer.o ../zht/TSafeQueue.o ../zht/ipFinder.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c /usr/local/lib/librestclient-cpp.la

.PHONY:	clean

clean:
	rm -rf ckp_client ckp_worker *.o *~
