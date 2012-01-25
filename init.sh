#!/bin/bash

adb -e shell mkdir /data/libvisual
adb -e shell mkdir /data/libvisual/plugins
adb -e shell mkdir /data/libvisual/plugins/input
adb -e shell mkdir /data/libvisual/plugins/actor
adb -e shell mkdir /data/libvisual/plugins/morph
adb -e shell mkdir /sdcard/starvisuals
adb -e push media/bg.bmp /mnt/sdcard/starvisuals/bg.bmp
