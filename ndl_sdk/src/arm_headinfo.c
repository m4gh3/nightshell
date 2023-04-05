#include <stdio.h>
#include <linux/elf.h>

#define print_sizeof(x)	printf("sizeof(%s): 0x%lx\n", #x, sizeof(x) )

int main()
{
	print_sizeof(Elf32_Rel);
	print_sizeof(Elf32_Sym);
	return 0;
}
