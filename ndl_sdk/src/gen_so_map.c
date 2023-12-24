#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


int is_header(const char *filename )
{ return strcmp(filename+strlen(filename)-2, ".h" ) == 0; }


void list_headers(FILE *out, const char *path, const char *inc_prefix )
{
	DIR *dir = opendir(path);
	if(dir)
	{
		struct dirent *entry;
		
		while( (entry = readdir(dir)) != NULL )
		{
			if(is_header(entry->d_name))
				fprintf(out, "#include <%s%s>\n", inc_prefix, entry->d_name );
		}
	}
}


int main()
{
	chdir("build");
	system("ar -x /opt/devkitpro/devkitARM/arm-none-eabi/lib/libc.a");
	system("ar -x /opt/devkitpro/devkitARM/arm-none-eabi/lib/libm.a");
	system("ar -x /opt/devkitpro/libnds/lib/libfat.a");
	system("ar -x /opt/devkitpro/libnds/lib/libnds9.a");
	system("ar -x /opt/devkitpro/libnds/lib/libmm9.a");
	system("rm libc_a-linkr.o libc_a-strlen-stub.o mm_main_ds.o");
	system("arm-none-eabi-gcc -I$DEVKITPRO/libnds/include ../src/libcplug.c *.o -shared -o ../lib/libcplug.so");
	system("rm *.o");

	FILE *pipe = popen("arm-none-eabi-objdump -T ../lib/libcplug.so", "r" );
	FILE *hout = fopen("../../ndl_loader/include/plink_map.hpp", "w" );

	char *include_paths[][2] =
	{
		{"/opt/devkitpro/devkitARM/arm-none-eabi/include", "" },
		{"/opt/devkitpro/devkitARM/arm-none-eabi/include/sys", "sys/" },
		{"/opt/devkitpro/libnds/include", "" }
	};
	for(int i=0; i < 3; i++ )
		list_headers(hout, include_paths[i][0], include_paths[i][1] );

	char str[256];
	fprintf(hout,	"#include <unordered_map>\n\n"
			"#define plink_pair(x) {#x, (void (*)())x }\n\n"
			"std::unordered_map<std::string, void (*)() > plink_map\n{\n"
	);
	while(1)
	{
		if( fscanf(pipe, "%s", str ) == EOF )
		{
			fprintf(hout, "};\n" );
			return 0;
		}
		if(!strcmp(".text", str ))
		{
			fscanf(pipe, "%s", str );
			fscanf(pipe, "%s", str );
			if(str[0] != '_' )
				fprintf(hout, "\tplink_pair(%s),\n", str );
		}
	}

	return 0;
}
