#ifndef GBAATM_FUNCTIONS
#define GBAATM_FUNCTIONS
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QString>
#include <QTextStream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <variables.h>

int ConvertKeys(char* keystr);
int fileexists(const char* filename);
void flippath(char* path);
void formatfopenstr(char* path);
void formatsystemstr(char* path);
void getpathfromfilename(char* filename, char* path);
void copyint(unsigned int* destint, unsigned int* srcint, int numint);
void strright(char* srcstr, char* deststr, unsigned int chartocpy);
void strleft(char* srcstr, char* deststr, unsigned int chartocpy);
void strmid(char* srcstr, char* deststr, unsigned int charstart, unsigned int chartocpy);
void stripnpc(char* stringtostrip);
void getromname(char* gbaromstr, char* tempname);
int findromend(unsigned int* gbaint, int gbaeof);
QString deadbeefrom(char* gbaromname, char* newgbaromname);
int patchrom(char* gbaromname, char* newgbaromname, unsigned int* mycheatint, int cheatintlen, int freeram, SLOMOSTRUCT slomostruct,
             ENABLEDISABLESTRUCT edstruct, int excycles, int wantmenu, unsigned int* menuint, int cheatselectram, bool vblankcheck,
             unsigned int* temptrainermenuint, int wantbg, int wantfont, int wantselect);
void goodname(char* badname);
int bmp2short(char* bmpfilestr, unsigned short* shortbuffer, int whichpic);
int hextoint(char* hexstr);
void cheatsCBAReverseArray(u8* array, u8* dest);
void chatsCBAScramble(u8* array, int count, u8 b);
u32 cheatsCBAGetValue(u8* array);
u16 cheatsCBAGetData(u8* array);
void cheatsCBAArrayToValue(u8* array, u8* dest);
void cheatsCBAParseSeedCode(u32 address, u32 value, u32* array);
u32 cheatsCBAEncWorker();
u32 cheatsCBACalcIndex(u32 x, u32 y);
void cheatsCBAUpdateSeedBuffer(u32 a, u8* buffer, int count);
void cheatsCBAChangeEncryption(u32* seed);
u16 cheatsCBAGenValue(u32 x, u32 y, u32 z);
void cheatsCBAGenTable();
u16 cheatsCBACalcCRC(u8* rom, int count);
void cheatsCBADecrypt(u8* decrypt);
void GSdecrypt(unsigned int* addressptr, unsigned int* valueptr);
short byteflip(short number);
int byteflipint(int number);
int testcht(char* cheatcodechar, char* srchstr);
char* formatcheats(char* cheatcodechar);
void getnextcheatline(char* cheatcodechar, int* chtptr, char* chtline);
void getnextchtline(char* cheatcodechar, int* chtptr, char* chtline);
void trim(char* textchar, char* texttotrim, char* replacechar);
int testchtline(char* cheatline);
int countcommas(char* textchar);
void importcht(char* cheatcodechar);
int convertraw(char* cheatcodes, unsigned int* cheatcodeint, int wantmenu, int cheatselectram, unsigned int* menuint);
int convertcb(char* cheatcodes, unsigned int* cheatcodeint, int wantmenu, int cheatselectram, unsigned int* menuint);

#endif
