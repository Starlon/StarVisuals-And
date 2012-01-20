#!/bin/bash
adb shell logcat -d -f /mnt/sdcard/test.log
adb pull /mnt/sdcard/test.log
./stack.py --symbols-dir=lyrical/ ./test.log 
