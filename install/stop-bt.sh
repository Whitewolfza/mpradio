#!/bin/bash

function log {
        sudo echo "[$(date)]: $*" >> /var/log/bluetooth_dev
}

for device in $( bt-device -l | grep -o "[[:xdigit:]:]\{11,17\}")
do
        log "Stop Played Connection " $device
        sudo systemctl stop mpradio-bt@$device
        killall mpradio && killall sox
        sudo systemctl start mpradio
done