#!/bin/bash
adb -d shell logcat -c
make clean
make debug
make installdebug
