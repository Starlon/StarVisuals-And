#!/bin/bash
adb -e shell logcat -c
ndk-build clean
ant clean
#ndk-build V=1 APP_OPTIM=debug
ndk-build
ant debug
adb -e install -r bin/starvisuals-debug.apk
adb -e push libs/armeabi/libinput_alsa.so /mnt/sdcard/starvisuals/plugins/input/
