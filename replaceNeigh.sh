##getting all the ips from amazon
echo "getting all the ip from the aws"
aws ec2 describe-instances --filters "Name=instance-type,Values=t2.medium" | awk '{print $4}' | grep 172.>neighbor.txt
##aremoving all the conf file
echo "removing any neghbor.conf file"
rm -rf neighbor.conf
####### creating the java file
javac Change.java
java Change $HOME/neighbor.txt 50000 60000 70000 80000
#java Change $HOME/neighbor.txt 60000 >>neighbor.conf
#java Change $HOME/neighbor.txt 70000 >>neighbor.conf
#java Change $HOME/neighbor.txt 80000 >>neighbor.conf

cat neighbor.conf
## sending changed value to all server
parallel-scp -h neighbor.txt -x "-o StrictHostKeyChecking=no -i DIC_CKP.pem" neighbor.conf ~/CloudKonPlus/src
