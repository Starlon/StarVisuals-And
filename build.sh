ndk-build clean
ant cleant
ndk-build -B V=1 APP_OPTIM=debug
ant debug
adb install -r bin/starvisuals-debug.apk
