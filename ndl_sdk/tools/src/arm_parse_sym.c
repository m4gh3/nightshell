#include <stdio.h>
#include <linux/elf.h>

#define print_sizeof(x)	printf("sizeof(%s): 0x%lx\n", #x, sizeof(x) )

int main()
{
	Elf32_Sym data;
	for(int i=0; fread(&data, sizeof(Elf32_Sym), 1, stdin ) > 0; i++ )
	{
		printf("@%d:\n\tst_name: 0x%x\n", i, data.st_name );
	}
	return 0;
}
