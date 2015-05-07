#!/bin/sh
sudo ldconfig

fusionfs fusion_root fusion_mount ../conf/neighbor.conf  ../conf/zht.conf
/home/arihant/Documents/Workspace/DIC/Code/CloudKonPlus/src/cloudkonplus/ckp_worker -n ../conf/neighbor.conf -z ../conf/zht.conf -t 1 &