cloudkonplus:
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c wc_client.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o wc_client wc_client.o ../zht/lock_guard.o ../zht/cpp_zhtclient.o ../zht/meta.pb-c.o ../zht/lru_cache.o ../zht/meta.pb.o ../zht/zpack.pb.o ../zht/novoht.o ../zht/bigdata_transfer.o ../zht/Const.o ../zht/ConfHandler.o ../zht/ConfEntry.o ../zht/ProxyStubFactory.o ../zht/proxy_stub.o ../zht/ip_proxy_stub.o ../zht/mq_proxy_stub.o ../zht/ipc_plus.o ../zht/tcp_proxy_stub.o ../zht/udp_proxy_stub.o ../zht/ZHTUtil.o ../zht/Env.o ../zht/Util.o ../zht/HTWorker.o ../zht/StrTokenizer.o ../zht/TSafeQueue.o ../zht/ipFinder.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -c wc_worker.cpp -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c
	gcc -g -I -L -DPF_INET -DBIG_MSG -DSOCKET_CACHE -DSCCB -DTSQUEUE -o wc_worker wc_worker.o ../zht/lock_guard.o ../zht/cpp_zhtclient.o ../zht/meta.pb-c.o ../zht/lru_cache.o ../zht/meta.pb.o ../zht/zpack.pb.o ../zht/novoht.o ../zht/bigdata_transfer.o ../zht/Const.o ../zht/ConfHandler.o ../zht/ConfEntry.o ../zht/ProxyStubFactory.o ../zht/proxy_stub.o ../zht/ip_proxy_stub.o ../zht/mq_proxy_stub.o ../zht/ipc_plus.o ../zht/tcp_proxy_stub.o ../zht/udp_proxy_stub.o ../zht/ZHTUtil.o ../zht/Env.o ../zht/Util.o ../zht/HTWorker.o ../zht/StrTokenizer.o ../zht/TSafeQueue.o ../zht/ipFinder.o -lstdc++ -lrt -lpthread -lm -lc -lprotobuf -lprotobuf-c

.PHONY:	clean

clean:
	rm -rf wc_client wc_worker *.o *~
