#include <stdio.h>
#include <string.h>
#include "soc1.h"
//-----------------------------------------------------------------------------//
void key_handler (void);
//-----------------------------------------------------------------------------//
//
//  Currently, data section was not generated in the bin, global variables are
//  all initialized to 0
//
int g_key = 0;

// /^^\ ;
#define OBJA 	0x5c5e5e2f 

// ####
#define OBJB 	0x23232323 

// >==<
#define BLAST	0x3c3d3d3e

// ||
#define BULLET	0x007c7c00

// D
#define LEFT	0x00000044
// C
#define RIGHT	0x00000043

// SPACE
#define SPACE	0x00000020

// update bullet, in cycles
#define BULLET_SPEED	300

// update BLAST, in cycles, should be much smaller than BULLET_SPEED
#define BLAST_SPEED	50

int g_currentpos = 0;
int g_bulletdelay = 0;
int g_blastdelay = 0;
int g_screen[8][10] = {0};
//
// Must be the first function in the .c file
// so that _start at the beginnign of the section
//
void _start(void)
{
	//int n = 0x30313233;

	// init
	int i = 0;
	int j = 0;

	*(int*)USER_KEY_HANDLER = (int)key_handler;

	*(int*)(TEXT_VIDEO_RAM_START) = 0x30303030;

	for (i = 0; i < 10; i++)
	{
		//g_screen[0][i] = OBJB;
		g_screen[1][i] = OBJB;
		g_screen[2][i] = OBJB;
		g_screen[3][i] = OBJB;
	}
	g_screen[7][5] = OBJA;
	g_currentpos = 5;
	g_bulletdelay = 0;
	g_blastdelay = 0;


	// main loop
	while(1)
	{
		// update key triggered event
		if (LEFT == g_key)
		{
			// clear key
			g_key = 0;
			// move to the left
			if (0 < g_currentpos)
			{
				g_screen[7][g_currentpos] = 0;
				g_currentpos--;
				g_screen[7][g_currentpos] = OBJA;

			}
		}
		else if (RIGHT == g_key)
		{
			// clear key
			g_key = 0;
			// move to the right
			if (g_currentpos < 9) // 9, bug: move out of sight
			{
				g_screen[7][g_currentpos] = 0;
				g_currentpos += 1;
				g_screen[7][g_currentpos] = OBJA;
			}
		}
		else if (SPACE == g_key)
		{	
			// clear key
			g_key = 0;
			g_screen[6][g_currentpos] = BULLET;
		}
		else
		{
			// do nothing
		}


		// update bullet move
		g_bulletdelay++;

		if (g_bulletdelay > BULLET_SPEED)
		{
			g_bulletdelay = 0;

			// every bullets move forward
			for (i = 1; i < 7; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (BULLET == g_screen[i][j])
					{
						g_screen[i][j] = 0;
						if (0 == g_screen[i-1][j])
						{
							g_screen[i-1][j] = BULLET;
						}
						else
						{
							// crash an object
							//g_screen[i-1][j] = BLAST;
							g_screen[i-1][j] = 0;
						}
					}
				}
			}
			// handle first line
			for (j = 0; j < 10; j++)
			{
				if (BULLET == g_screen[0][j])
				{
					g_screen[0][j] = 0;
				}
			}
		}

		// update blast
		g_blastdelay++;
		if (g_blastdelay > BLAST_SPEED)
		{
			g_blastdelay = 0;

			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < 10; j++)
				{
					if (BLAST == g_screen[i][j])
					{
						//g_screen[i][j] = 0;
						g_screen[0][0] = 0;
					}
				}
			}

		}


		// update screen

		for (i = 0; i < 8; i++)
		{
			for (j = 0; j < 10; j++)
			{
				//*(int*)((int)TEXT_VIDEO_RAM_START + (i+2)*TEXT_COLUMN + j*4) = g_screen[i][j];
				*(int*)((int)TEXT_VIDEO_RAM_START + (i+2)*TEXT_COLUMN + j*4) = g_screen[i][j];
				//*(int*)((int)TEXT_VIDEO_RAM_START + 80*2) = 0x23232323;
			}
		}
	}

	// tmp test code
	//	asm("addi x2, x0, 0xFF"); // user stack at 0xff00
	//        asm("slli x2, x2, 0x8");

	//	((void (*) (void)) 0x1000) ();


	return;
}


void key_handler (void)
{
	int n = 0;

	n = *(int*)UARTDR;
	*(int*)(TEXT_VIDEO_RAM_START) = n;
	g_key = n;
}
