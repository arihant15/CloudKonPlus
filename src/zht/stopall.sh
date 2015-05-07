echo "kill zhtserver"
fuser -k 50000/tcp
fuser -k 60000/tcp
fuser -k 70000/tcp
fuser -k 80000/tcp

killall -9 peerserver

