#!/bin/bash
./objdump.sh
adb -e shell logcat -d -f /mnt/sdcard/test.log
adb -e pull /mnt/sdcard/test.log
#./stack.py --symbols-dir=lyrical/ ./test.log 
./parse_stack.py ./libstarvisuals.asm ./test.log
tail ./test.log
