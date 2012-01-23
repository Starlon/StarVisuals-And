adb forward tcp:5039 tcp:5039
adb shell /system/bin/gdbserver tcp:5039 --attach PID
