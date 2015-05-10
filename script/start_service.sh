sudo ldconfig
#start the service for file transfer
ffsnetd 2>&1 1>/dev/null &  
echo "ffsnetd started"
sleep 0
#start the service for ZHT server
#zhtserver -n ../conf/neighbor.conf -z ../conf/zht.conf 2>&1 1>/dev/null &
$HOME/CloudKonPlus/src/zht/zhtserver -n $HOME/CloudKonPlus/conf/neighbor.conf -z $HOME/CloudKonPlus/conf/zht.conf 2>&1 1>/dev/null &
echo "ZHT Server started"
sleep 0
