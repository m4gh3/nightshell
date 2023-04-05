#include <stdio.h>
#include <linux/elf.h>

#define print_sizeof(x)	printf("sizeof(%s): 0x%lx\n", #x, sizeof(x) )

int main()
{
	Elf32_Rel data;
	for(int i=0; fread(&data, sizeof(Elf32_Rel), 1, stdin ) > 0; i++ )
	{
		printf("@%d:\n\tr_offset: 0x%x\n\tr_info: 0x%x\n", i, data.r_offset, data.r_info );
	}
	return 0;
}
