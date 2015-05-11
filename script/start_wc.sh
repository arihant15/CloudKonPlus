sudo ldconfig

fusionfs fusion_root fusion_mount $HOME/CloudKonPlus/conf/neighbor.conf  $HOME/CloudKonPlus/conf/zht.conf &
echo "FusionFS Started started"
sleep 0
$HOME/CloudKonPlus/src/wordcount/wc_worker -n $HOME/CloudKonPlus/conf/neighbor.conf -z $HOME/CloudKonPlus/conf/zht.conf -t 1 &
echo "CKP Worker Started started"
sleep 0
