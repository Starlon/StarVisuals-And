#!/bin/bash

adb -d shell mkdir /data/local/libvisual
adb -d shell mkdir /data/local/libvisual/plugins
adb -d shell mkdir /data/local/libvisual/plugins/input
adb -d shell mkdir /data/local/libvisual/plugins/actor
adb -d shell mkdir /data/local/libvisual/plugins/morph
adb -d shell mkdir /sdcard/starvisuals
adb -d push media/bg.bmp /mnt/sdcard/starvisuals/bg.bmp
