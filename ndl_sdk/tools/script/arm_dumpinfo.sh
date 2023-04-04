#!/bin/sh

BASEPATH=$(dirname $(readlink -f $0))/..

readelf -a $1
arm-none-eabi-objdump -dj .got.plt $1
echo .got.plt data dump:
arm-none-eabi-objcopy -O binary -j .got.plt $1 /dev/stdout | xxd
arm-none-eabi-objdump -dj .plt $1
echo .rel.plt data dump:
arm-none-eabi-objcopy -O binary -j .rel.plt $1 /dev/stdout | $BASEPATH/bin/arm_parse_reloc
echo .dynsym data dump:
arm-none-eabi-objcopy -O binary -j .dynsym $1 /dev/stdout | $BASEPATH/bin/arm_parse_sym
echo .dynstr data dump:
arm-none-eabi-objcopy -O binary -j .dynstr $1 /dev/stdout | xxd
