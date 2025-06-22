//#include <libcplug.h>
#define ARM9
#include <nds.h>
#include <math.h>
#include <stdio.h>

volatile float frame = 0;

//---------------------------------------------------------------------------------
void Vblank() {
//---------------------------------------------------------------------------------
	frame++;
}	

void plug_main()
{

	iprintf("      Hello DS dev'rs\n");
	iprintf("     \x1b[32mwww.devkitpro.org\n");
	iprintf("   \x1b[32;1mwww.drunkencoders.com\x1b[39m");
 
	while(1)
	{

		touchPosition touchXY;

		//irqSet(IRQ_VBLANK, Vblank );

		swiWaitForVBlank();
		frame++; //sigh
		scanKeys();
		int keys = keysDown();
		if (keys & KEY_START) break;

		touchRead(&touchXY);

		// print at using ansi escape sequence \x1b[line;columnH 
		printf("\x1b[10;0HFrame = %f", 0.5f*frame );
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);		
	
	}

}
