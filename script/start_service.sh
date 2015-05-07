sudo ldconfig
#start the service for file transfer
ffsnetd 2>&1 1>/dev/null &  

#start the service for ZHT server
#zhtserver -n ../conf/neighbor.conf -z ../conf/zht.conf 2>&1 1>/dev/null &
/home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/src/zht/zhtserver -n ../conf/neighbor.conf -z ../conf/zht.conf 2>&1 1>/dev/null &
