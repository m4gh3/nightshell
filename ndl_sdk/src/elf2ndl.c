#include <linux/elf.h>

#include <limits.h>
#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>

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


int main(int argc, char *argv[] )
{
    if( argc < 3 )
    {
        fprintf(stderr, "Usage: elf2ndl input.elf output.ndl\n" );
        return EX_USAGE;
    }
    FILE *in_file = fopen(argv[1], "r" );
    if( in_file == NULL )
    {
        fprintf(stderr, "could not open %s", argv[1] );
        return EX_NOINPUT;
    }
    size_t in_sz; fseek(in_file, 0, SEEK_END ); in_sz = ftell(in_file); fseek(in_file, 0, SEEK_SET );
    char *in_buffer = malloc(in_sz);
    if( in_buffer == NULL )
    {
        fprintf(stderr, "could not allocate buffer to read the whole file into" );
        return EX_OSERR;
    }
    size_t read_n = fread(in_buffer, 1, in_sz, in_file );
    if( read_n < in_sz )
    {
        fprintf(stderr, "could not read the whole file for some reason" );
        return EX_IOERR;
    }
    Elf32_Ehdr *header = (Elf32_Ehdr *)in_buffer;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(in_buffer+header->e_shoff);
    printf("header.e_shoff: %d\nheader.e_shstrndx: %d\n", header->e_shoff, header->e_shstrndx );
    Elf32_Shdr *shstr_shdr = (Elf32_Shdr *)(shdr+header->e_shstrndx);
    printf("shstr_shdr.sh_offset: %d\n", shstr_shdr->sh_offset );
    char *shstr_table = in_buffer+shstr_shdr->sh_offset; 
    printf("\ncreating the ndl header:\n");
    ndl_header out_header;

#define print_sh_name_off_sz(x) printf("\tfound section %s : rel_addr 0x%x size 0x%x\n", x, shdr[i].sh_addr, shdr[i].sh_size )

    strcpy(out_header.magic, "ndl" ); out_header.magic[3] = 0;
    out_header.version = 0;
    for(Elf32_Half i=0; i < header->e_shnum; i++ )
    {
        char* str = shstr_table + shdr[i].sh_name;
        if( !strcmp(".got.plt", str ) )
        {
            print_sh_name_off_sz(".got.plt");
            out_header.got_offset = shdr[i].sh_addr;
            out_header.got_size = shdr[i].sh_size;
        }
        else if( !strcmp(".plt", str ) )
        {
            print_sh_name_off_sz(".plt");
            out_header.plt_offset = shdr[i].sh_addr;
            out_header.plt_size = shdr[i].sh_size;
        }
        else if( !strcmp(".rel.plt", str ) )
        {
            print_sh_name_off_sz(".rel.plt");
            out_header.rel_plt_offset = shdr[i].sh_addr;
            out_header.rel_plt_size = shdr[i].sh_size;
        }
        else if( !strcmp(".dynsym", str ) )
        {
            print_sh_name_off_sz(".dynsym");
            out_header.dynsym_offset = shdr[i].sh_addr;
            out_header.dynsym_size = shdr[i].sh_size;
        }
        else if( !strcmp(".dynstr", str ) )
        {
            print_sh_name_off_sz(".dynstr");
            out_header.dynstr_offset = shdr[i].sh_addr;
            out_header.dynsym_size = shdr[i].sh_size;
        }
    }

    fclose(in_file);
    free(in_buffer);

    FILE *out_file = fopen(argv[2], "w" );
    char objcopy_cmd[0x100+PATH_MAX+PATH_MAX];
    char tmp_path[PATH_MAX+1]; tmpnam(tmp_path);
    printf("\nobjcopy tmp file: %s\n", tmp_path );
    snprintf(objcopy_cmd, 0xff+PATH_MAX+PATH_MAX, "arm-none-eabi-objcopy -O binary %s %s", argv[1], tmp_path );
    system(objcopy_cmd);
    printf("executing: %s\n", objcopy_cmd );
    FILE *objcopy_in = fopen(tmp_path, "r" );
    fwrite(&out_header, sizeof(ndl_header), 1, out_file );

    for(int c; (c=fgetc(objcopy_in)) != -1; )
        fputc(c, out_file );

    fclose(out_file);
    snprintf(objcopy_cmd, 0xff+PATH_MAX, "rm %s", tmp_path );
    printf("executing: %s\n", objcopy_cmd );
    system(objcopy_cmd);
    printf("\ndone.\n");

    return 0;

}
