#!/bin/bash

adb -e install -r bin/starvisuals-debug.apk
adb -e shell mkdir /mnt/sdcard/starvisuals
adb -e shell mkdir /mnt/sdcard/starvisuals/plugins
adb -e shell mkdir /mnt/sdcard/starvisuals/plugins/input
adb -e push libs/armeabi/libinput_alsa.so /mnt/sdcard/starvisuals/plugins/input
adb -e push media/bg.bmp /mnt/sdcard/starvisuals/bg.bmp
