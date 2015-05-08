sudo ldconfig

fusionfs fusion_root fusion_mount $HOME/CloudKonPlus/conf/neighbor.conf  $HOME/CloudKonPlus/conf/zht.conf
$HOME/CloudKonPlus/src/cloudkonplus/ckp_worker -n $HOME/CloudKonPlus/conf/neighbor.conf -z $HOME/CloudKonPlus/conf/zht.conf -t 1 &
