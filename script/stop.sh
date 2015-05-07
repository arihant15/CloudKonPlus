#!/bin/sh
sudo ldconfig

fusermount -u fusion_mount
rm fusionfs.log fusion_mount/* fusion_root/* -rf
