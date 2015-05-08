sudo ldconfig

fusermount -u $HOME/CloudKonPlus/script/fusion_mount
rm fusionfs.log $HOME/CloudKonPlus/script/fusion_mount/* $HOME/CloudKonPlus/script/fusion_root/* -rf
