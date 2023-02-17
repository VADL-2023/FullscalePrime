(rtl_fm -M fm -f 144.97M -s 24000 -d 00000001 -g 42 - 2>/dev/null & echo $! > rtl_fm_pid) | direwolf -c /home/pi/FullscalePrime/sdr1.conf -r 24000 -D 1 -B 1200 - > $1 &
cat rtl_fm_pid