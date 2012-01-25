#!/bin/bash
adb -e install -r bin/StarVisuals.apk
adb -e push libs/armeabi/libinput_alsa.so /mnt/sdcard/starvisuals/plugins/input/
adb -e push libs/armeabi/libactor_corona.so /mnt/sdcard/starvisuals/plugins/actor/
