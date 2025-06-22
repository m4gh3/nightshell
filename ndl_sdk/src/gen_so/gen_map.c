#include <stdio.h>
#include <string.h>

int main()
{
	char str[256];
	printf("std::unordered_map<std::string, void (*)() > plink_map\n{\n");
	while(1)
	{
		if( scanf("%s", str ) == EOF )
		{
			printf("};\n");
			return 0;
		}
		if(!strcmp(".text", str ))
		{
			scanf("%s", str );
			scanf("%s", str );
			if(str[0] != '_' )
				printf("\t{\"%s\", %s}, \n", str, str );
		}
	}
}
