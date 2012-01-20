#!/bin/bash
ndk-build clean
ant clean
ndk-build -B V=1 APP_OPTIM=debug
ant debug
adb install -r bin/starvisuals-debug.apk
