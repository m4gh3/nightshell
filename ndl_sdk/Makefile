all: lib/libcplug.so lib/plug_entry.o bin/gen_so_map bin/elf2ndl bin/arm_parse_reloc bin/arm_parse_sym

#dummy library to link to
lib/libcplug.so: bin/gen_so_map src/libcplug.c
	#arm-none-eabi-gcc -mthumb -nostdlib -shared src/libcplug.c -o lib/libcplug.so
	./bin/gen_so_map
	#mv build/libcplug.so lib/libcplug.so

#this contains the plugin startup/initialization assembly code
lib/plug_entry.o: src/plug_entry.s
	arm-none-eabi-as src/plug_entry.s -c -o lib/plug_entry.o

#gen_so_map creates both the dummy library and the header containing the ndl_loader hashmap
bin/gen_so_map: src/gen_so_map.c
	gcc src/gen_so_map.c -g -o bin/gen_so_map

#elf2ndl opens the plugin compiled to an elf, reads the data from it and outputs a plugin in the .ndl format
bin/elf2ndl: src/elf2ndl.c
	gcc src/elf2ndl.c -o bin/elf2ndl

#arm_parse_reloc is a tool that is used to parse relocation tables for dynamic linking, used to develop elf2ndl
bin/arm_parse_reloc: src/arm_parse_reloc.c
	gcc src/arm_parse_reloc.c -o bin/arm_parse_reloc

#arm_parse_sym is a tool that is used to parse symbols used for dynamic linking, used to develop elf2ndl
bin/arm_parse_sym: src/arm_parse_sym.c
	gcc src/arm_parse_sym.c -o bin/arm_parse_sym

clean:
	rm lib/*
	rm bin/*

#arm-none-eabi-gcc -mthumb -fPIC $1.c -c -o $1.o
##arm-none-eabi-ld -pie -nostdlib -T $BASEPATH/plugscript.ld $BASEPATH/lib/plug_entry.o -L$BASEPATH/lib -lcplug $@
#elf2ndl $1 $1.ndl
