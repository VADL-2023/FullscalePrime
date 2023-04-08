#!/bin/bash
pgrep direwolf > direwolf_pids.txt
while read p; do
    echo "Killing "$p
    sudo kill -9 $p
done <direwolf_pids.txt

pgrep rtl > rtl_pids.txt
while read p; do
    echo "Killing "$p
    sudo kill -9 $p
done <rtl_pids.txt