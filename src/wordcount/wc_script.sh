cat $1 | tr -cs A-Za-z\' '\n' | tr A-Z a-z | sort | uniq -c | sort -k1,1nr -k2 >> $2
