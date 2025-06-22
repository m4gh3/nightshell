#!/usr/bin/sh

cd build
ar -x /opt/devkitpro/devkitARM/arm-none-eabi/lib/libc.a
ar -x /opt/devkitpro/devkitARM/arm-none-eabi/lib/libm.a
ar -x /opt/devkitpro/libnds/lib/libfat.a
ar -x /opt/devkitpro/libnds/lib/libnds9.a
ar -x /opt/devkitpro/libnds/lib/libmm9.a
rm libc_a-linkr.o libc_a-strlen-stub.o mm_main_ds.o
arm-none-eabi-gcc *.o -shared -o ../libcplug.so
rm *.o
DIRS="$DEVKITARM/arm-none-eabi/include\
	$DEVKITARM/arm-none-eabi/include/sys/\
	$DEVKITARM/arm-none-eabi/include\
	$DEVKITPRO/libnds/include"

for dir in $DIRS #$DEVKITARM/arm-none-eabi/include $DEVKITPRO/libnds/include
do
	cd $dir
	for file in *.h
	do
		echo $file
	done
done
