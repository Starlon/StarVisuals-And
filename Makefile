# Android Makefile v0.1 (c) 2011 - Daniel Hiepler <daniel@niftylight.de>

APPNAME = StarVisuals
ACTIVITY = starvisuals

all:
	@ndk-build
	@ant clean
	@ant release
install:
	@adb -e install -r bin/StarVisuals.apk
	@make upload

debug:
	@ndk-build APP_OPTIM=debug
	@ant clean
	@ant debug

debuginstall:
	@adb -e install -r bin/starvisuals-debug.apk
	@make upload

upload:
	@adb -e push libs/armeabi/libinput_alsa.so /data/local/libvisual/plugins/input/
	#@adb -e push libs/armeabi/libactor_AVS.so /data/local/libvisual//plugins/actor/
	#@adb -e push libs/armeabi/libactor_avs_stars.so /data/local/libvisual/plugins/actor/
	#@adb -e push libs/armeabi/libactor_avs_superscope.so /data/local/libvisual/plugins/actor/
	@adb -e push libs/armeabi/libactor_corona.so /data/local/libvisual/plugins/actor/
	@adb -e push libs/armeabi/libactor_lv_scope.so /data/local/libvisual/plugins/actor/

clean:
	@ndk-build clean
	@ant clean

update:
	@android update project --path . --target android-10

keygen:
	@keytool -genkey -v -keystore my.keystore -alias $(APPNAME)_key -keyalg RSA -keysize 4096 -validity 100000

sign:
	@jarsigner -keystore my.keystore bin/$(ACTIVITY)-release-unsigned.apk $(APPNAME)_key
	@zipalign -v 4 bin/$(ACTIVITY)-release-unsigned.apk bin/$(APPNAME).apk

init:
	@adb -e shell mkdir /data/local/libvisual
	@adb -e shell mkdir /data/local/libvisual/plugins
	@adb -e shell mkdir /data/local/libvisual/plugins/input
	@adb -e shell mkdir /data/local/libvisual/plugins/actor
	@adb -e shell mkdir /data/local/libvisual/plugins/morph
	@adb -e shell mkdir /sdcard/starvisuals
	@adb -e push media/bg.bmp /mnt/sdcard/starvisuals/bg.bmp
