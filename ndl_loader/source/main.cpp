#include <filesystem.h>
#include <nds.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "elf.h"
#include <unordered_map>
#include <string>
#include <fat.h>


extern "C" char _ndl_runtime_resolver0_[];


typedef struct ndl_header
{
	char magic[4];
	uint32_t version;
	uint32_t entry;
	uint32_t got_offset;
	uint32_t got_size;
	uint32_t plt_offset;
	uint32_t plt_size;
	uint32_t rel_plt_offset;
	uint32_t rel_plt_size;
	uint32_t dynsym_offset;
	uint32_t dynsym_size;
	uint32_t dynstr_offset;
	uint32_t dynstr_size;
} ndl_header;


#define plink_pair(x) {#x, (void (*)())x }

std::unordered_map<std::string, void (*)() > plink_map
{ plink_pair(puts) };

extern "C" void (*_ndl_runtime_resolver1_(int *got,  ndl_header *header, int func_id ))()
{
	char *imageContent = (char *)(header+1);
	int sym_idx = (((Elf32_Rel *)(imageContent+header->rel_plt_offset))[func_id].r_info >> 8);
	int name_idx = ((Elf32_Sym *)(imageContent+header->dynsym_offset))[sym_idx].st_name;
	int patch_r_offset = (((Elf32_Rel *)(imageContent+header->rel_plt_offset))[func_id].r_offset);
	std::string func_name(imageContent+header->dynstr_offset+name_idx);
	void (*func_ptr)() = plink_map[func_name];
	*((void (**)())(imageContent+patch_r_offset)) = (void (*)())func_ptr;
	return (void (*)())func_ptr;
}

char *load_plugin(char *filename )
{
	FILE *ndl_image = fopen(filename, "r" );
	if(ndl_image == NULL )
		return NULL;
	else
	{
		size_t ndl_sz; fseek(ndl_image, 0, SEEK_END ); ndl_sz = ftell(ndl_image); fseek(ndl_image, 0, SEEK_SET );
		char *imageBytes = (char *)malloc(ndl_sz);
		fread(imageBytes, 1, ndl_sz, ndl_image );
		ndl_header *imageHeader = (ndl_header *)imageBytes;
		char *imageContent = imageBytes+sizeof(ndl_header);
		*(void **)(imageContent+imageHeader->got_offset) = (void *)imageHeader;
		*(void **)(imageContent+imageHeader->got_offset+8) = (void *)_ndl_runtime_resolver0_;
		for(uint32_t i=0xc; i < imageHeader->got_size; i+=0x4 )
		{ *(void **)(imageContent+imageHeader->got_offset+i) = (void *)(imageContent+imageHeader->plt_offset); }
		return imageBytes;
	}
}

void call_plug_main(char *imageBytes)
{ ((void (*)())(imageBytes+sizeof(ndl_header)))(); }

int main(void)
{

	consoleDemoInit();
	//nitroFSInit(NULL);
	fatInitDefault();
	char *imageBytes = load_plugin("hello_plugin.ndl");
	if(imageBytes == NULL)
	{
		printf("could not open plugin!!!\n");
		while(1);
	}	
	while(1)
	{
		swiWaitForVBlank();
		scanKeys();
		int pressed = keysDown();
		if(pressed & KEY_START)
		{
			call_plug_main(imageBytes);
		}
	}

}
