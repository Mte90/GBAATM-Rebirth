#ifndef GBAATM_VARIABLES
#define GBAATM_VARIABLES
#include <QString>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define CHEAT_IS_HEX(a) (((a) >= 'A' && (a) <= 'F') || ((a) >= '0' && (a) <= '9'))
#define SIZEOFHOOKJUMP 15 // 10 //7
#define MAXCHTLINE 4000
#define MAXCODELEN 40960
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

struct ENABLEDISABLESTRUCT
{
  int wantenable;
  int enablekey;
  int disablekey;
  char enablekeystr[50];
  char disablekeystr[50];
};

struct SLOMOSTRUCT
{
  int wantslomo;
  int slowdownkey;
  int speedupkey;
  char slowdownkeystr[50];
  char speedupkeystr[50];
};

struct CUSTOMIZE
{
  int font;
  int background;
  int selectionbar;
};

struct ADDRESSSTRUCT
{
  unsigned int* oldasmaddrs;
  unsigned int* asmaddr;
};

struct LVALSTRUCT
{
  unsigned int* oldasmlvalues;
  unsigned int* asmlvalue;
};

ENABLEDISABLESTRUCT myedstruct;
SLOMOSTRUCT myslomostruct;
CUSTOMIZE customizetrainer;
unsigned short* menubgshort = (unsigned short*)malloc(76800);
unsigned short* menuselectshort = (unsigned short*)malloc(6720);
unsigned short* menufontshort = (unsigned short*)malloc(216);
u8 cheatsCBASeedBuffer[0x30];
u32 cheatsCBASeed[4];
u32 cheatsCBATemporaryValue = 0;
u16 cheatsCBATable[256];
bool cheatsCBATableGenerated = false;
u8 cheatsCBACurrentSeed[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned int seed0 = 0x9F4FBBD;
unsigned int seed1 = 0x9681884A;
unsigned int seed2 = 0x352027E9;
unsigned int seed3 = 0xF3DEE5A7;

#endif
