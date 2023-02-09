(rtl_fm -M fm -f 144.97M -s 24000 -d $1 -g $2 -l $3 - 2>/dev/null & echo $! > rtl_fm_pid) | direwolf -c ~/sdr-1200bps.conf -r 24000 -D 1 -B 1200 - > $4 &
cat rtl_fm_pid