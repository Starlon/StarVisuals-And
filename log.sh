#!/bin/bash
./objdump.sh
adb shell logcat -d -f /mnt/sdcard/test.log
adb pull /mnt/sdcard/test.log
#./stack.py --symbols-dir=lyrical/ ./test.log 
./parse_stack.py ./libstarvisuals.asm ./test.log
tail ./test.log
