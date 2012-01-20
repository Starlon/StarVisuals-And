adb shell mount -o rw,remount -t yaffs2 /dev/block/mtdblock03 /system  
adb shell chmod 06755 /system  
adb shell chmod 06755 /system/xbin/su
