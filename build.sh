#!/bin/bash
adb -e shell logcat -c
ndk-build clean
ant clean
ndk-build -B V=1 APP_OPTIM=debug
ant debug
adb -e install -r bin/starvisuals-debug.apk
ndk-gdb -e --start --force
