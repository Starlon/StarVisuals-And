# Android Makefile v0.1 (c) 2011 - Daniel Hiepler <daniel@niftylight.de>

APPNAME = StarVisuals
ACTIVITY = starvisuals

all:
	@ndk-build
	@ant clean
	@ant release
install:
	@adb -d install -r bin/StarVisuals.apk
	@make upload

debug:
	@ndk-build APP_OPTIM=debug
	@ant clean
	@ant debug

debuginstall:
	@adb -d install -r bin/starvisuals-debug.apk
	@make upload

upload:
	@adb -d push libs/armeabi/libinput_alsa.so /data/local/libvisual/plugins/input/
	@adb -d push libs/armeabi/libactor_AVS.so /data/local/libvisual//plugins/actor/
	@adb -d push libs/armeabi/libactor_avs_stars.so /data/local/libvisual/plugins/actor/
	@adb -d push libs/armeabi/libactor_avs_superscope.so /data/local/libvisual/plugins/actor/
	@adb -d push libs/armeabi/libactor_corona.so /data/local/libvisual/plugins/actor/
	@adb -d push libs/armeabi/libactor_lv_scope.so /data/local/libvisual/plugins/actor/

clean:
	@ndk-build clean
	@ant clean

update:
	@android update project --path .

keygen:
	@keytool -genkey -v -keystore my.keystore -alias $(APPNAME)_key -keyalg RSA -keysize 4096 -validity 100000

sign:
	@jarsigner -keystore my.keystore bin/$(ACTIVITY)-release-unsigned.apk $(APPNAME)_key
	@zipalign -v 4 bin/$(ACTIVITY)-release-unsigned.apk bin/$(APPNAME).apk

