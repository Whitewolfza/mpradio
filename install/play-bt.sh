#!/bin/bash

function log {
	sudo echo "[$(date)]: $*" >> /var/log/bluetooth_dev
}


BTMAC=$1
bluetoothctl <<< 'connect '$BTMAC
log "Start Played Connection " $BTMAC
sed -i "s/^defaults.bluealsa.device.*$/defaults.bluealsa.device \"$BTMAC\"/g" /home/pi/.asoundrc
sudo systemctl stop mpradio
killall mpradio && killall sox
sudo systemctl start mpradio-bt@$BTMAC