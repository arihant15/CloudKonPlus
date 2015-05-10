cat sim_* | sort -k2 | awk -F\  '{a[$2]+=$1} END {for (i in a) print i"  "a[i]}' | sort >> output.txt
