#!/bin/bash
adb -d shell logcat -c
ndk-build clean
ant clean
#ndk-build V=1 APP_OPTIM=debug
ndk-build
ant debug
adb -d install -r bin/starvisuals-debug.apk
adb -d push libs/armeabi/libinput_alsa.so /mnt/sdcard/starvisuals/plugins/input/
