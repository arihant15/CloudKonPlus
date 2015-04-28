echo "this would work from the home directory"
javac Change.java
java Change $HOME/neghbor.txt 5000
parallel-scp -h neighbor.txt -x "-o StrictHostKeyChecking=no -i DIC_CKP.pem" neighbor.conf ~/CloudKonPlus/src
