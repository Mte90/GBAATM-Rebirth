#include "core/trainermenu.h"
#include <QString>
#include <QTextStream>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
unsigned short* menubgshort = (unsigned short*)malloc(76800);
unsigned short* menuselectshort = (unsigned short*)malloc(6720);
unsigned short* menufontshort = (unsigned short*)malloc(216);
#define MAXCHTLINE 4000
#define MAXCODELEN 40960
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

u8 cheatsCBASeedBuffer[0x30];
u32 cheatsCBASeed[4];
u32 cheatsCBATemporaryValue = 0;
u16 cheatsCBATable[256];
bool cheatsCBATableGenerated = false;

u8 cheatsCBACurrentSeed[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define CHEAT_IS_HEX(a)                                                        \
  (((a) >= 'A' && (a) <= 'F') || ((a) >= '0' && (a) <= '9'))

#define SIZEOFHOOKJUMP 10 // 7
int
ConvertKeys(char* keystr)
{
  char* keys[] = { "L",     "R",     "DOWN",   "UP", "LEFT",
                   "RIGHT", "START", "SELECT", "B",  "A" };
  int keycode = 0x3ff;
  for (int toupperptr = 0; toupperptr < strlen(keystr); toupperptr++) {
    keystr[toupperptr] = toupper(keystr[toupperptr]);
  }

  char tempstr[100];
  tempstr[0] = 0;
  for (int thiskey = 0; thiskey < 10; thiskey++) {
    if (strstr(keystr, keys[thiskey])) {
      if ((thiskey == 0) || (thiskey == 1) || (thiskey == 9)) {

        if (thiskey == 0) {
          if (*(strstr(keystr, keys[0]) + 1) != 'E') {
            keycode ^= 1 << (9 - thiskey);
            sprintf(tempstr + strlen(tempstr), "+%s", keys[thiskey]);
          }
        }

        if (thiskey == 1) {
          if ((*(strstr(keystr, keys[1]) + 1) != 'I') &&
              (*(strstr(keystr, keys[1]) + 1) != 'T')) {
            keycode ^= 1 << (9 - thiskey);
            sprintf(tempstr + strlen(tempstr), "+%s", keys[thiskey]);
          }
        }

        if (thiskey == 9) {
          if (*(strstr(keystr, keys[9]) - 2) != 'S') {
            keycode ^= 1 << (9 - thiskey);
            sprintf(tempstr + strlen(tempstr), "+%s", keys[thiskey]);
          }
        }

      } else {
        if (((thiskey == 3) && (!strstr(keystr, keys[2]))) ||
            ((thiskey == 5) && (!strstr(keystr, keys[4]))) ||
            ((thiskey < 3) || (thiskey == 4) || (thiskey > 5))) {
          keycode ^= 1 << (9 - thiskey);
          sprintf(tempstr + strlen(tempstr), "+%s", keys[thiskey]);
        }
      }
    }
  }
  if (tempstr[0] == '+') {
    sprintf(keystr, "%s", tempstr + 1);
  } else {
    sprintf(keystr, "%s", tempstr);
  }

  return keycode;
}

int
fileexists(const char* filename)
{
  FILE* thisfile = fopen(filename, "rb");
  if (thisfile) {
    fclose(thisfile);
    return 1;
  } else {
    return 0;
  }
}

int
hextoint(char* hexstr)
{
  int pos = 0;
  char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
                    'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f' };
  int returnval = -1;
  while (hexstr[pos] != 0) {
    for (int i = 0; i < 22; i++) {
      if (hexstr[pos] == hexmap[i]) {
        if (returnval == -1) {
          returnval = 0;
        }
        returnval <<= 4;
        if (i < 16) {
          returnval |= i;
        } else {
          returnval |= (i - 6);
        }
      }
    }
    pos++;
  }
  return returnval;
}

void
flippath(char* path)
{
  for (int strptr = 0; strptr < strlen(path); strptr++) {
    if (path[strptr] == '\\') {
      path[strptr] = (char)'/';
    } else if (path[strptr] == '/') {
      path[strptr] = (char)'\\';
    }
  }
}

void
formatfopenstr(char* path)
{
  for (int strptr = 0; strptr < strlen(path); strptr++) {
    if (path[strptr] == '\\') {
      path[strptr] = (char)'/';
    }
  }
}

void
formatsystemstr(char* path)
{
  for (int strptr = 0; strptr < strlen(path); strptr++) {
    if (path[strptr] == '\\') {
      path[strptr] = (char)'/';
    }
  }
}

void
getpathfromfilename(char* filename, char* path)
{
  int lastslash;
  lastslash = -1;
  for (int charptr = 0; charptr < strlen(filename); charptr++) {
    if ((filename[charptr] == '\\') || (filename[charptr] == '/')) {
      lastslash = charptr;
    }
  }
  memcpy(path, filename, lastslash + 1);
  path[lastslash + 1] = 0;
}

void
copyint(unsigned int* destint, unsigned int* srcint, int numint)
{
  for (int intptr = 0; intptr < numint; intptr++) {
    *(destint + intptr) = *(srcint + intptr);
  }
}
void
strright(char* srcstr, char* deststr, unsigned int chartocpy)
{
  for (unsigned int charcpy = strlen(srcstr) - chartocpy;
       charcpy < strlen(srcstr);
       charcpy++) {
    deststr[charcpy - (strlen(srcstr) - chartocpy)] = srcstr[charcpy];
  }
  deststr[chartocpy] = 0;
}

void
strleft(char* srcstr, char* deststr, unsigned int chartocpy)
{
  for (unsigned int charcpy = 0; charcpy < chartocpy; charcpy++) {
    deststr[charcpy] = srcstr[charcpy];
  }
  deststr[chartocpy] = 0;
}

void
strmid(char* srcstr,
       char* deststr,
       unsigned int charstart,
       unsigned int chartocpy)
{
  for (unsigned int charcpy = 0; charcpy < chartocpy; charcpy++) {
    deststr[charcpy] = srcstr[charcpy + charstart - 1];
  }
  deststr[chartocpy] = 0;
}

void
stripnpc(char* stringtostrip)
{
  char* tempstr = (char*)malloc(strlen(stringtostrip) + 100);
  memset(tempstr, 0, strlen(stringtostrip) + 100);
  int tempptr = 0;
  for (int charptr = 0; charptr < strlen(stringtostrip); charptr++) {
    char thischar = *(stringtostrip + charptr);

    if ((thischar >= 0x20) && (thischar <= 0x7a)) {
      *(tempstr + tempptr) = thischar;
      tempptr++;
    }

    if (thischar == 0xa) {
      *(tempstr + tempptr) = '\r';
      *(tempstr + tempptr + 1) = '\n';
      tempptr += 2;
    }
  }
  memset(stringtostrip, 0, strlen(stringtostrip) + 1);
  memcpy(stringtostrip, tempstr, strlen(tempstr) + 1);
  free(tempstr);
}

void
getromname(char* gbaromstr, char* tempname)
{
  char* gbachar = (char*)malloc(0xb0);
  formatfopenstr(gbaromstr);
  FILE* gbaromfile = fopen(gbaromstr, "rb");
  if (gbaromfile) {
    fread(gbachar, 1, 0xb0, gbaromfile);
    fclose(gbaromfile);
    memset(tempname, 0, 25);
    memcpy(tempname, gbachar + 0xa0, 12);
    sprintf(tempname + strlen(tempname), " - [");
    memcpy(tempname + strlen(tempname), gbachar + 0xac, 4);
    sprintf(tempname + strlen(tempname), "]");
    free(gbachar);
  }
}

int
findromend(unsigned int* gbaint, int gbaeof)
{
  int lastnonpad = -1;
  for (int gbaptr = 0; gbaptr < (gbaeof / 4) - 1; gbaptr++) {
    // if (gbaint[gbaptr]!=padint) { lastnonpad=gbaptr; }
    if (((gbaint[gbaptr] != 0xffffffff) && (gbaint[gbaptr] != 0x0)) ||
        (gbaint[gbaptr] != gbaint[gbaptr + 1])) {
      lastnonpad = gbaptr;
    }
  }
  return (lastnonpad)*4;
}
/*
void findhooks(int * gbaint,int gbaeof) {
for(int gbaptr=0; gbaptr<(gbaeof-gbaIDlen)/4; gbaptr++) {

if (((gbaint[gbaptr]&0xffff0fff)==0xe3a00301) &&
((gbaint[gbaptr+1]&0xfff00fff)==0xe2800c02) &&
((gbaint[gbaptr+2]&0xfff00fff)==0xe5900000)) {


        printf("Irq address found at 0x%X using
r%d\n\n",0x8000000+gbaptr*4,((gbaint[gbaptr]&0xf000)>>12));
}

}
}
*/

QString
deadbeefrom(char* gbaromname, char* newgbaromname)
{
  unsigned int gbadeadbeefint[] = { 0xE59F0018, 0xE3A01403, 0xE3A02A02,
                                    0xE4810004, 0xE2522001, 0x1AFFFFFC,
                                    0xE51FF004, 0x0,        0xEFBEADDE };

#define SIZEOFDBFUNC 9

  char tempchar[300];
  QString code = "-1";
  unsigned int* gbaromint = NULL;
  formatfopenstr(gbaromname);
  FILE* gbafile = fopen(gbaromname, "rb");
  if (gbafile) {
    fseek(gbafile, 0, SEEK_END);
    int gbalen = ftell(gbafile);
    rewind(gbafile);
    gbaromint = (unsigned int*)malloc(gbalen + SIZEOFDBFUNC * 4);
    fread(gbaromint, 1, gbalen, gbafile);
    unsigned int fillint = *(gbaromint + (gbalen / 4) - 1);
    if ((fillint != 0xffffffff) && (fillint != 0x0)) {
      fillint = 0x0;
    }
    for (int fillctr = 0; fillctr < 0xe; fillctr++) {
      *(gbaromint + (gbalen / 4) + fillctr) = fillint;
    }
    fclose(gbafile);
    int realgbaend = findromend(gbaromint, gbalen + SIZEOFDBFUNC * 4);
    sprintf(tempchar, "Free space found at 0x%X", realgbaend + 0x8000004);
    QTextStream(stdout) << tempchar;

    copyint(gbaromint + (realgbaend + 4) / 4, gbadeadbeefint, SIZEOFDBFUNC);
    *(gbaromint + (realgbaend + 4) / 4 + SIZEOFDBFUNC - 2) =
      0x8000000 | ((*gbaromint & 0xffffff) * 4 + 8);
    *gbaromint = 0xea000000 | ((realgbaend / 4) - 1);
    if (fileexists(newgbaromname) == 1) {
      remove(newgbaromname);
    }
    formatfopenstr(newgbaromname);
    FILE* newgbaromfile = fopen(newgbaromname, "wb");
    if (newgbaromfile) {
      realgbaend = findromend(gbaromint, gbalen + SIZEOFDBFUNC * 4);
      int byteswritten = fwrite(gbaromint, 1, realgbaend + 4, newgbaromfile);
      fclose(newgbaromfile);
      if (byteswritten != realgbaend + 4) {
        free(gbaromint);
        return code;
      }
    } else {
      free(gbaromint);
      return code;
    }
  } else {
    free(gbaromint);
    return code;
  }

  free(gbaromint);
  return tempchar;
}

int
patchrom(char* gbaromname,
         char* newgbaromname,
         unsigned int* mycheatint,
         int cheatintlen,
         int freeram,
         SLOMOSTRUCT slomostruct,
         ENABLEDISABLESTRUCT edstruct,
         int excycles,
         char* path,
         int wantmenu,
         unsigned int* menuint,
         int cheatselectram,
         bool vblankcheck,
         unsigned int* temptrainermenuint)
{

  unsigned int eddisint[] = { 0xE59F103C, 0xE5D14000, 0xE1DF23BA, 0xE1DF33B4,
                              0xE3A00301, 0xE5900130, 0xE3A05C03, 0xE28550FF,
                              0xE0000005, 0xE1500002, 0x3A04001,  0xE1500003,
                              0x3A04000,  0xE5C14000, 0xE3540000, 0x1A000002,
                              0xE12FFF1E, 0xDDDDDDDD, 0xEEEEFFFF };

  unsigned int edenint[] = { 0xE59F103C, 0xE5D14000, 0xE1DF23BA, 0xE1DF33B4,
                             0xE3A00301, 0xE5900130, 0xE3A05C03, 0xE28550FF,
                             0xE0000005, 0xE1500002, 0x3A04000,  0xE1500003,
                             0x3A04001,  0xE5C14000, 0xE3540001, 0x1A000002,
                             0xE12FFF1E, 0xDDDDDDDD, 0xEEEEFFFF };

  unsigned int slomoint[] = {
    0xE3A02000, 0xE1DF47BE, 0xE1DF57B8, 0xE3A01301, 0xE5911130, 0xE3A03C03,
    0xE28330FF, 0xE0011003, 0xE59F605C, 0xE5D60002, 0xE1510004, 0x3A02001,
    0x2800002,  0xE35000FE, 0xC3A000FE, 0xE1510005, 0x3A02001,  0x2500002,
    0xB3A00000, 0xE5D61001, 0xE3510000, 0x5C60002,  0xE3500000, 0xA00000A,
    0xE5C62001, 0xE3A01088, 0xE1A00000, 0xE1A00000, 0xE2511001, 0x1AFFFFFB,
    0xE2500001, 0x3A000002, 0x2AFFFFF7, 0xDDDDDDDD, 0xEEEEFFFF
  };
  unsigned int execint[] = { 0xE59F101C, 0xE5D12003, 0xE3A03000, 0xE2822001,
                             0xE1520003, 0x3A02000,  0xE5C12003, 0xA000001,
                             0xE12FFF1E, 0xFFFFFFFF };
  unsigned int vblankint[] = { 0xE59F100C, 0xE5910000, 0xE35000A0,
                               0xAA000001, 0xE12FFF1E, 0x4000206 };
  unsigned int trainerigmint[] = { 0xE3A01301, 0xE591B130, 0xE59F2008,
                                   0xE15B0002, 0xA000000,  0xEA000000,
                                   0x35b }; // select+down+left

#define TRAINERINTMAX 0x4000

  int menupatch = 0;
  int menustart = 0;

  char tempchar[300];

  unsigned int* trainerint = (unsigned int*)malloc(TRAINERINTMAX);
  int trainerintptr = 0;
  memset(trainerint, 0, TRAINERINTMAX);
  formatfopenstr(gbaromname);
  FILE* gbafile = fopen(gbaromname, "rb");

  if (gbafile) {
    fseek(gbafile, 0, SEEK_END);
    int gbalen = ftell(gbafile);
    rewind(gbafile);
    unsigned int* gbaromint = (unsigned int*)malloc(gbalen + 0x20000);
    fread(gbaromint, 1, gbalen, gbafile);
    unsigned int fillint = *(gbaromint + (gbalen / 4) - 1);
    if ((fillint != 0xffffffff) && (fillint != 0x0))
      fillint = 0x0;
    for (int fillctr = 0; fillctr < 0x8000; fillctr++) {
      *(gbaromint + (gbalen / 4) + fillctr) = fillint;
    }
    fclose(gbafile);
    int realgbaend = findromend(gbaromint, gbalen + 0x20000);
    sprintf(tempchar, "Free space found at 0x%X", realgbaend + 0x8000004);

    QTextStream(stdout) << tempchar;
    int spaceneeded = 4;
    if (cheatintlen > 0)
      spaceneeded = cheatintlen;
    spaceneeded += 924;
    if (wantmenu == 1)
      spaceneeded += trainermenuint[0] + 28 + 340;
    if (excycles > 1)
      spaceneeded += 40;
    if (edstruct.wantenable == 1)
      spaceneeded += 76;
    if (slomostruct.wantslomo == 1)
      spaceneeded += 132;
    if (!vblankcheck)
      spaceneeded -= 6;

    int gbahookaddr[10];
    int gbahookreg[10];
    int gbahooktype[10];
    int gbahooks = -1;

    int temphookaddr = 0;
    int temphookreg = 0;

    char regtaken[16];
    memset(regtaken, 0, 16);

    for (int gbaptr = 0; gbaptr < (realgbaend - 12) / 4; gbaptr++) {

      // new hook detect
      if (((gbaromint[gbaptr] & 0xffff0fff) == 0xe3a00301 /*mov r[5th],*/) &&
          ((gbaromint[gbaptr + 1] & 0xfff00fff) ==
           0xe2800c02 /*add 4th,5th - badreg*/) &&
          ((gbaromint[gbaptr + 2] & 0xfff00fff) ==
           0xe5d00008 /*ldr 4th,5th - badreg*/) &&
          ((gbaromint[gbaptr + 2] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/)) {
        temphookaddr = gbaptr * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1;
      }
      // end new hook detect

      if (((gbaromint[gbaptr] & 0xffff0fff) == 0xe3a00301 /*mov r[5th],*/) &&
          ((gbaromint[gbaptr + 1] & 0xfff00fff) ==
           0xe2800c02 /*add 4th,5th - badreg*/) &&
          ((gbaromint[gbaptr + 2] & 0xfff00fff) ==
           0xe5900000 /*ldr 4th,5th - badreg*/) &&
          ((gbaromint[gbaptr + 2] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/)) {
        temphookaddr = gbaptr * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1;
      }

      if (((gbaromint[gbaptr] & 0xffff0000) == 0xe92d0000 /*push*/) &&
          ((gbaromint[gbaptr + 1] & 0xffff0fff) ==
           0xe3a00301 /*mov r[5th],*/) &&
          ((gbaromint[gbaptr + 2] & 0xfff00fff) ==
           0xe5b00200 /*ldr - 4th,5th = bad reg*/) &&
          ((gbaromint[gbaptr + 3] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/)) {
        temphookaddr = (gbaptr + 1) * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1; // 2;
      }

      if (((gbaromint[gbaptr] & 0xffff0fff) == 0xe3a00640 /*mov 5th],*/) &&
          ((gbaromint[gbaptr + 1] & 0xfff00fff) ==
           0xe5b00200 /*ldr - 4th,5th = bad reg*/) &&
          ((gbaromint[gbaptr + 2] & 0xfff00000) ==
           0xe1d00000 /*ldr? - 4th,5th = bad reg*/) &&
          ((gbaromint[gbaptr + 5] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/) &&
          ((gbaromint[gbaptr + 6] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/) &&
          ((gbaromint[gbaptr + 7] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/)) {
        temphookaddr = (gbaptr + 5) * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1; // 3;
      }

      if (((gbaromint[gbaptr] & 0xffff0fff) == 0xe3a00301) &&
          ((gbaromint[gbaptr + 1] & 0xfff00fff) == 0xe5b00200) &&
          ((gbaromint[gbaptr + 2] & 0xfff00fff) == 0xe1d000b8)) {
        temphookaddr = gbaptr * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1; // 4;
      }

      if (((gbaromint[gbaptr] & 0xffff0000) == 0xe59f0000 /*ldr rX,[r15,+]*/) &&
          ((gbaromint[gbaptr + 1] & 0xfff000ff) == 0xe5900000 /*ldr rX*/) &&
          ((gbaromint[gbaptr + 1] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/) &&
          ((gbaromint[gbaptr + 2] & 0xffff0000) == 0xe1a00000 /*mov*/) &&
          ((gbaromint[gbaptr + 3] & 0xffff0000) !=
           0xe59f0000 /*ldr rX,[r15,+]*/)) {
        temphookaddr = (gbaptr + 1) * 4;
        temphookreg = (gbaromint[gbaptr] & 0xf000) >> 12;
        gbahooktype[gbahooks + 1] = 1; // long jump
      }

      if (temphookaddr != 0) {
        gbahooks++;
        gbahookaddr[gbahooks] = temphookaddr;
        gbahookreg[gbahooks] = temphookreg;
        sprintf(tempchar, "IRQ found at 0x%X", temphookaddr + 0x8000000);
        temphookaddr = 0;
        temphookreg = 0;
        if (gbahooks == 9)
          break;
      }
    }

    if (gbahooks == -1) {
      sprintf(tempchar, "No IRQs found!");
      QTextStream(stdout) << tempchar;
      free(gbaromint);
      return -1;
    }

    spaceneeded += (gbahooks + 1) * SIZEOFHOOKJUMP * 4;
    spaceneeded = ((int)((spaceneeded + 3) / 4)) * 4;

    if ((realgbaend + spaceneeded) > 0x1000000) {
      QTextStream(stdout)
        << "The game will be larger than 16MB. Many flash carts have a 16MB "
           "limit if the games run from PSRAM so it may not work!";
    }

    if ((realgbaend + spaceneeded) > 0x2000000) {
      QTextStream(stdout)
        << "The max size a GBA game can be is 32MB. There is not enough space "
           "at the end of this game.\r\nThe game will be trimmed to the proper "
           "size but it may result graphics corruption, etc.";
      realgbaend = 0x2000000 - spaceneeded;
      sprintf(tempchar, "The game was trimmed to 0x8%07X", realgbaend * 4);
      QTextStream(stdout) << tempchar;
    }

    int oktopatch = 1;

    for (int hookctr = 0; hookctr < gbahooks + 1; hookctr++) {

      for (int opctr = 0; opctr < 4; opctr++) {
        int thisop = gbaromint[gbahookaddr[hookctr] / 4 + opctr];

        if (((thisop & 0xf0000) == 0xd0000) || ((thisop & 0xf000) == 0xd000))
          oktopatch = -1;
      }

      if (oktopatch == -1) {
        sprintf(tempchar,
                "Patch not applied to %08",
                gbahookaddr[hookctr] + 0x8000000);
        QTextStream(stdout) << tempchar;
        continue;
      }

      *(trainerint + trainerintptr) = 0xE92D48FF; // push r0-r7,r11,r14
      *(trainerint + trainerintptr + 1) =
        0xEB000000 | (((gbahooks - hookctr) * SIZEOFHOOKJUMP +
                       (SIZEOFHOOKJUMP - 3))); // bl trainerfunc

      *(trainerint + trainerintptr + 2) = 0xE51D005A; // ldr r0, [r13, -5A]
      *(trainerint + trainerintptr + 3) = 0xE2800008; // add r0, 8
      *(trainerint + trainerintptr + 4) = 0xE50D005A; // str r0, [r13, -5A]

      *(trainerint + trainerintptr + 5) = 0xE8BD48FF; // pop r0-r7,r11,r14
      *(trainerint + trainerintptr + 6) = gbaromint[gbahookaddr[hookctr] / 4];
      *(trainerint + trainerintptr + 7) =
        gbaromint[gbahookaddr[hookctr] / 4 + 1];
      *(trainerint + trainerintptr + 8) =
        gbaromint[gbahookaddr[hookctr] / 4 + 2];

      *(trainerint + trainerintptr + 9) = 0xE8BD8000; // pop r15

      gbaromint[gbahookaddr[hookctr] / 4] = 0xE92D8000; // push r15
      gbaromint[gbahookaddr[hookctr] / 4 + 1] =
        0xE51FF004; // ldr r15,traineraddr
      gbaromint[gbahookaddr[hookctr] / 4 + 2] =
        realgbaend + 0x8000004 + hookctr * SIZEOFHOOKJUMP * 4; // traineraddr
      trainerintptr += SIZEOFHOOKJUMP;
    }

    if (vblankcheck == 1) {
      copyint(trainerint + trainerintptr, vblankint, 6);
      trainerintptr += 6;
    }

    if (excycles > 1) {
      copyint(trainerint + trainerintptr, execint, 10);
      *(trainerint + trainerintptr + 2) |= excycles;
      *(trainerint + trainerintptr + 9) = freeram;
      trainerintptr += 10;
    }
    if (edstruct.wantenable == 1) {
      copyint(trainerint + trainerintptr, eddisint, 19);
      *(trainerint + trainerintptr + 17) = freeram;
      *(trainerint + trainerintptr + 18) =
        (edstruct.enablekey << 16) | edstruct.disablekey;
      trainerintptr += 19;
    }
    if (slomostruct.wantslomo == 1) {
      copyint(trainerint + trainerintptr, slomoint, 35);
      trainerintptr += 35;
      *(trainerint + trainerintptr - 2) = freeram;
      *(trainerint + trainerintptr - 1) =
        (slomostruct.slowdownkey << 16) | slomostruct.speedupkey;
    }
    if (wantmenu == 1) {
      copyint(trainerint + trainerintptr, trainerigmint, 7);
      *(trainerint + trainerintptr + 4) |= cheatintlen + 6;
      trainerintptr += 7;
    }

    if (cheatintlen > 0) {
      copyint(trainerint + trainerintptr, mycheatint, cheatintlen);
      trainerintptr += cheatintlen;
    } else {
      *(trainerint + trainerintptr) = 0xE12FFF1E;
      trainerintptr++;
    }

    int savejump = 0;
    if (wantmenu == 1) {
      sprintf(tempchar,
              "Menu placed at 0x%X - trainerintptr = 0x%X",
              0x8000000 + realgbaend + 4 + trainerintptr * 4,
              trainerintptr * 4);
      QTextStream(stdout) << tempchar;
      savejump = *gbaromint;
      *gbaromint = 0xEA000000 | (((realgbaend + 4) / 4 + trainerintptr) - 2);
    }

    copyint(gbaromint + (realgbaend + 4) / 4, trainerint, trainerintptr);

    if (wantmenu == 1) {

      int searchptr = 0;
      int menupatchoffset = 0;
      for (int searchptr = 0; searchptr < 0x200; searchptr++) {
        if (*(temptrainermenuint + searchptr + 1) == 0x8000000) {
          menupatchoffset = searchptr;
          break;
        }
      }
      if (wantbg == 1) {
        char* bgptr = (char*)temptrainermenuint;
        bgptr += (*(temptrainermenuint + searchptr + 7) & 0xffffff) + 8;
        memcpy(bgptr, (char*)menubgshort, 76800);
      }
      if (wantselect == 1) {
        char* selectptr = (char*)temptrainermenuint;
        selectptr += (*(temptrainermenuint + searchptr + 5) & 0xffffff) + 8;
        memcpy(selectptr, (char*)menuselectshort, 6720);
      }
      if (wantfont == 1) {
        char* fontptr = (char*)temptrainermenuint;
        fontptr += (*(temptrainermenuint + searchptr + 2) & 0xffffff) + 4;
        memcpy(fontptr, (char*)menufontshort, 216);
      }
      for (int trainerptr = 0; trainerptr < 9; trainerptr++) {
        *(temptrainermenuint + menupatchoffset + trainerptr + 1) =
          (*(temptrainermenuint + menupatchoffset + trainerptr + 1) &
           0xffffff) +
          0x8000000 + realgbaend + 4 + trainerintptr * 4;
      }
      copyint(gbaromint + ((realgbaend + 4) / 4) + trainerintptr,
              temptrainermenuint + 1,
              (*trainermenuint) / 4); // TRAINERMENULEN/4);
      *(gbaromint + ((realgbaend + 4) / 4) + trainerintptr + 2) =
        0xE1A00000; // menupatch;
      *(gbaromint + ((realgbaend + 4) / 4) + trainerintptr + 4) =
        0x8000000 | ((savejump & 0xffffff) * 4 + 8);
      *(gbaromint + ((realgbaend + 4) / 4) + trainerintptr + menupatchoffset +
        9) = cheatselectram;
      copyint(gbaromint + ((realgbaend + 4) / 4) + trainerintptr +
                (*temptrainermenuint) / 4,
              menuint,
              0x400); // TRAINERMENULEN/4,menuint,0x400);
    }

    if (fileexists(newgbaromname) == 1)
      remove(newgbaromname);
    formatfopenstr(newgbaromname);
    FILE* newgbaromfile = fopen(newgbaromname, "wb");
    if (newgbaromfile) {
      realgbaend = findromend(gbaromint, gbalen + 0x20000);
      int byteswritten = fwrite(gbaromint, 1, realgbaend + 4, newgbaromfile);
      free(gbaromint);
      int currlen = ftell(newgbaromfile);
      unsigned char padbuff[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                                  0xff, 0xff, 0xff, 0xff };
      int bytewritenum = 16 - (currlen % 16);
      if (bytewritenum == 0)
        bytewritenum = 16;
      int byteswritten2 = fwrite(padbuff, 1, bytewritenum, newgbaromfile);
      fclose(newgbaromfile);
      if ((byteswritten == realgbaend + 4) && (byteswritten2 == bytewritenum)) {
        QTextStream(stdout) << "The patched game was written successfully!";
      } else {
        QTextStream(stdout) << "There was a problem writing the game!";
      }
    } else {
      QTextStream(stdout) << "The file could not be written!";
    }
  }

  return 1;
}

void
cheatsCBAReverseArray(u8* array, u8* dest)
{
  dest[0] = array[3];
  dest[1] = array[2];
  dest[2] = array[1];
  dest[3] = array[0];
  dest[4] = array[5];
  dest[5] = array[4];
}

void
chatsCBAScramble(u8* array, int count, u8 b)
{
  u8* x = array + (count >> 3);
  u8* y = array + (b >> 3);
  u32 z = *x & (1 << (count & 7));
  u32 x0 = (*x & (~(1 << (count & 7))));
  if (z != 0)
    z = 1;
  if ((*y & (1 << (b & 7))) != 0)
    x0 |= (1 << (count & 7));
  *x = x0;
  u32 temp = *y & (~(1 << (b & 7)));
  if (z != 0)
    temp |= (1 << (b & 7));
  *y = temp;
}

u32
cheatsCBAGetValue(u8* array)
{
  return array[0] | array[1] << 8 | array[2] << 16 | array[3] << 24;
}

u16
cheatsCBAGetData(u8* array)
{
  return array[4] | array[5] << 8;
}

void
cheatsCBAArrayToValue(u8* array, u8* dest)
{
  dest[0] = array[3];
  dest[1] = array[2];
  dest[2] = array[1];
  dest[3] = array[0];
  dest[4] = array[5];
  dest[5] = array[4];
}

void
cheatsCBAParseSeedCode(u32 address, u32 value, u32* array)
{
  array[0] = 1;
  array[1] = value & 0xFF;
  array[2] = (address >> 0x10) & 0xFF;
  array[3] = (value >> 8) & 0xFF;
  array[4] = (address >> 0x18) & 0x0F;
  array[5] = address & 0xFFFF;
  array[6] = address;
  array[7] = value;
}

u32
cheatsCBAEncWorker()
{
  u32 x = (cheatsCBATemporaryValue * 0x41c64e6d) + 0x3039;
  u32 y = (x * 0x41c64e6d) + 0x3039;
  u32 z = x >> 0x10;
  x = ((y >> 0x10) & 0x7fff) << 0x0f;
  z = (z << 0x1e) | x;
  x = (y * 0x41c64e6d) + 0x3039;
  cheatsCBATemporaryValue = x;
  return z | ((x >> 0x10) & 0x7fff);
}

#define ROR(v, s) (((v) >> (s)) | (((v) & ((1 << (s)) - 1)) << (32 - (s))))

u32
cheatsCBACalcIndex(u32 x, u32 y)
{
  if (y != 0) {
    if (y == 1)
      x = 0;
    else if (x == y)
      x = 0;
    if (y < 1)
      return x;
    else if (x < y)
      return x;
    u32 x0 = 1;

    while (y < 0x10000000) {
      if (y < x) {
        y = y << 4;
        x0 = x0 << 4;
      } else
        break;
    }

    while (y < 0x80000000) {
      if (y < x) {
        y = y << 1;
        x0 = x0 << 1;
      } else
        break;
    }

  loop:
    u32 z = 0;
    if (x >= y)
      x -= y;
    if (x >= (y >> 1)) {
      x -= (y >> 1);
      z |= ROR(x0, 1);
    }
    if (x >= (y >> 2)) {
      x -= (y >> 2);
      z |= ROR(x0, 2);
    }
    if (x >= (y >> 3)) {
      x -= (y >> 3);
      z |= ROR(x0, 3);
    }

    u32 temp = x0;

    if (x != 0) {
      x0 = x0 >> 4;
      if (x0 != 0) {
        y = y >> 4;
        goto loop;
      }
    }

    z = z & 0xe0000000;

    if (z != 0) {
      if ((temp & 7) == 0)
        return x;
    } else
      return x;

    if ((z & ROR(temp, 3)) != 0)
      x += y >> 3;
    if ((z & ROR(temp, 2)) != 0)
      x += y >> 2;
    if ((z & ROR(temp, 1)) != 0)
      x += y >> 1;
    return x;
  } else {
  }
  // should not happen in the current code
  return 0;
}

void
cheatsCBAUpdateSeedBuffer(u32 a, u8* buffer, int count)
{
  int i;
  for (i = 0; i < count; i++)
    buffer[i] = i;
  for (i = 0; (u32)i < a; i++) {
    u32 a = cheatsCBACalcIndex(cheatsCBAEncWorker(), count);
    u32 b = cheatsCBACalcIndex(cheatsCBAEncWorker(), count);
    u32 t = buffer[a];
    buffer[a] = buffer[b];
    buffer[b] = t;
  }
}

void
cheatsCBAChangeEncryption(u32* seed)
{
  int i;

  cheatsCBATemporaryValue = (seed[1] ^ 0x1111);
  cheatsCBAUpdateSeedBuffer(0x50, cheatsCBASeedBuffer, 0x30);
  cheatsCBATemporaryValue = 0x4efad1c3;

  for (i = 0; (u32)i < seed[4]; i++) {
    cheatsCBATemporaryValue = cheatsCBAEncWorker();
  }
  cheatsCBASeed[2] = cheatsCBAEncWorker();
  cheatsCBASeed[3] = cheatsCBAEncWorker();

  cheatsCBATemporaryValue = seed[3] ^ 0xf254;

  for (i = 0; (u32)i < seed[3]; i++) {
    cheatsCBATemporaryValue = cheatsCBAEncWorker();
  }

  cheatsCBASeed[0] = cheatsCBAEncWorker();
  cheatsCBASeed[1] = cheatsCBAEncWorker();

  *((u32*)&cheatsCBACurrentSeed[0]) = seed[6];
  *((u32*)&cheatsCBACurrentSeed[4]) = seed[7];
  *((u32*)&cheatsCBACurrentSeed[8]) = 0;
}

u16
cheatsCBAGenValue(u32 x, u32 y, u32 z)
{
  y <<= 0x10;
  z <<= 0x10;
  x <<= 0x18;
  u32 x0 = (int)y >> 0x10;
  z = (int)z >> 0x10;
  x = (int)x >> 0x10;
  for (int i = 0; i < 8; i++) {
    u32 temp = z ^ x;
    if ((int)temp >= 0) {
      temp = z << 0x11;
    } else {
      temp = z << 0x01;
      temp ^= x0;
      temp = temp << 0x10;
    }
    z = (int)temp >> 0x10;
    temp = x << 0x11;
    x = (int)temp >> 0x10;
  }
  return z & 0xffff;
}

void
cheatsCBAGenTable()
{
  for (int i = 0; i < 0x100; i++) {
    cheatsCBATable[i] = cheatsCBAGenValue(i, 0x1021, 0);
  }
  cheatsCBATableGenerated = true;
}

u16
cheatsCBACalcCRC(u8* rom, int count)
{
  u32 crc = 0xffffffff;

  if (count & 3) {
    // 0x08000EAE
  } else {
    count = (count >> 2) - 1;
    if (count != -1) {
      while (count != -1) {
        crc = (((crc << 0x08) ^
                cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++])
               << 0x10) >>
              0x10;
        crc = (((crc << 0x08) ^
                cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++])
               << 0x10) >>
              0x10;
        crc = (((crc << 0x08) ^
                cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++])
               << 0x10) >>
              0x10;
        crc = (((crc << 0x08) ^
                cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++])
               << 0x10) >>
              0x10;
        count--;
      }
    }
  }
  return crc & 0xffff;
}

void
cheatsCBADecrypt(u8* decrypt)
{
  u8 buffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  u8* array = &buffer[1];

  cheatsCBAReverseArray(decrypt, array);

  for (int count = 0x2f; count >= 0; count--) {
    chatsCBAScramble(array, count, cheatsCBASeedBuffer[count]);
  }
  cheatsCBAArrayToValue(array, decrypt);
  *((u32*)decrypt) = cheatsCBAGetValue(decrypt) ^ cheatsCBASeed[0];
  *((u16*)(decrypt + 4)) =
    (cheatsCBAGetData(decrypt) ^ cheatsCBASeed[1]) & 0xffff;

  cheatsCBAReverseArray(decrypt, array);

  u32 cs = cheatsCBAGetValue(cheatsCBACurrentSeed);
  for (int i = 0; i <= 4; i++) {
    array[i] = ((cs >> 8) ^ array[i + 1]) ^ array[i];
  }

  array[5] = (cs >> 8) ^ array[5];

  for (int j = 5; j >= 0; j--) {
    array[j] = (cs ^ array[j - 1]) ^ array[j];
  }

  cheatsCBAArrayToValue(array, decrypt);

  *((u32*)decrypt) = cheatsCBAGetValue(decrypt) ^ cheatsCBASeed[2];
  *((u16*)(decrypt + 4)) =
    (cheatsCBAGetData(decrypt) ^ cheatsCBASeed[3]) & 0xffff;
}

unsigned int seed0 = 0x9F4FBBD;
unsigned int seed1 = 0x9681884A;
unsigned int seed2 = 0x352027E9;
unsigned int seed3 = 0xF3DEE5A7;

void
GSdecrypt(unsigned int* addressptr, unsigned int* valueptr)
{
  unsigned int address = *addressptr;
  unsigned int value = *valueptr;
  unsigned int seed = 0xC6EF3720;
  int decrypt = 32;
  while (decrypt > 0) {
    value -= ((((address << 4) + seed2) ^ (address + seed)) ^
              ((address >> 5) + seed3));
    address -=
      ((((value << 4) + seed0) ^ (value + seed)) ^ ((value >> 5) + seed1));
    seed -= 0x9E3779B9;
    decrypt--;
  }
  *addressptr = address;
  *valueptr = value;
}

short
byteflip(short number)
{
  return (((number >> 8) & 0xff) | ((number << 8) & 0xff00)) & 0xffff;
}

int
byteflipint(int number)
{
  return (((number >> 24) & 0xff) | ((number >> 8) & 0xff00) |
          ((number << 8) & 0xff0000) | ((number << 24) & 0xff000000));
}

int
testcht(char* cheatcodechar, char* srchstr)
{
  char* tempchar = (char*)malloc(strlen(cheatcodechar) + 10);
  char* tempsrch = (char*)malloc(strlen(srchstr) + 10);
  memset(tempchar, 0, strlen(cheatcodechar) + 10);
  memset(tempsrch, 0, strlen(srchstr) + 10);
  char thischar;
  int charptr = 0;
  for (int charptr = 0; charptr < strlen(cheatcodechar); charptr++) {
    thischar = *(cheatcodechar + charptr);
    if ((thischar >= 0x61) && (thischar <= 0x7a))
      thischar -= 0x20;
    *(tempchar + charptr) = thischar;
  }

  for (charptr = 0; charptr < strlen(srchstr); charptr++) {
    thischar = *(srchstr + charptr);
    if ((thischar >= 0x61) && (thischar <= 0x7a))
      thischar -= 0x20;
    *(tempsrch + charptr) = thischar;
  }
  int match = 0;
  if (strstr(tempchar, tempsrch))
    match = 1;
  free(tempsrch);
  free(tempchar);
  return match;
}

void
formatcheats(char* cheatcodechar)
{
  char goodcodechar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8',
                          '9', 'A', 'B', 'C', 'D', 'E', 'F', ' ' };
  char lastchar[] = { 0, 0 };
  char thischar[] = { 0, 0 };
  int labellast = 0;
  char* lastgood;
  char* templine = (char*)malloc(500);
  memset(templine, 0, 500);
  char* tempchar = (char*)malloc(strlen(cheatcodechar) * 5);
  memset(tempchar, 0, strlen(cheatcodechar) * 5);
  int howmanylines = 0;
  strcat(cheatcodechar, "\r\n");
  for (unsigned int strptr = 0; strptr < strlen(cheatcodechar); strptr++) {
    thischar[0] = *(cheatcodechar + strptr);

    if (thischar[0] == '\t')
      thischar[0] = '\n';
    if ((thischar[0] == ':') || (thischar[0] == '-')) {
      thischar[0] = ' ';
    }

    if (((thischar[0] < 0x20) || (thischar[0] > 0x7e)) &&
        (thischar[0] != 0xa) && (thischar[0] != 0x9)) {
      lastchar[0] = thischar[0];
    }
    if ((thischar[0] >= 0x61) && (thischar[0] <= 0x7a)) {
      thischar[0] -= 0x20;
    }

    if (lastchar[0] != thischar[0]) {
      if ((thischar[0] != 0x9) && (thischar[0] != 0xd)) {
        strcat(templine, thischar);
        lastchar[0] = thischar[0];
      }

      if (thischar[0] == 0xa) {
        if (templine[0] != '/') {
          howmanylines++;
          int goodchars = 0;
          for (unsigned int charptr = 0; charptr < strlen(templine);
               charptr++) {
            for (unsigned int goodcharptr = 0;
                 goodcharptr < strlen(goodcodechar);
                 goodcharptr++) {
              if (templine[charptr] == goodcodechar[goodcharptr]) {
                goodchars++;
              }
            }
          }

          if ((goodchars >= 10) && ((strlen(templine) - goodchars) < 5)) {
            if (goodchars < 20) {
              char tempcpychar[2];
              memset(tempcpychar, 0, 2);
              char* newtempline = (char*)malloc(strlen(templine) + 20);
              memset(newtempline, 0, strlen(templine) + 7);
              int tempcpyptr = 0;
              while ((tempcpychar[0] != ' ') &&
                     (tempcpyptr < strlen(templine))) {
                tempcpychar[0] = *(templine + tempcpyptr);
                if (tempcpychar[0] != ' ') {
                  strcat(newtempline, tempcpychar);
                }
                tempcpyptr++;
              }
              if (tempcpyptr == strlen(templine)) {
                tempcpyptr = strlen(templine) - 5;
                *(newtempline + strlen(templine) - 5) = 0;
              }
              strcat(newtempline, " ");
              memset(newtempline + strlen(newtempline),
                     0,
                     strlen(templine) + 20 - strlen(newtempline));
              memset(newtempline + tempcpyptr + 1,
                     '0',
                     9 - (strlen(templine) - tempcpyptr));
              sprintf(
                newtempline + strlen(newtempline), "%s", templine + tempcpyptr);
              labellast = 0;
              sprintf(templine, newtempline);

              free(newtempline);
            }
            strcat(tempchar, templine);
          } else { // label or bad code
            if (strlen(templine) > 0) {
              templine[strlen(templine) - 1] = 0;
              if ((labellast == 0) && (howmanylines > 1)) {
                strcat(tempchar, "}\n");
              }
              if (labellast == 1) {
                memset(lastgood, 0, 100);
              }
              lastgood = tempchar + strlen(tempchar);
              sprintf(tempchar + strlen(tempchar), "[%s] \n{\n", templine);
              labellast = 1;
            }
          }
        }
        memset(templine, 0, 500);
      }

    } else {
      lastchar[0] = thischar[0];
      if ((thischar[0] != 0x20) && (thischar[0] != 0xa)) {
        strcat(templine, thischar);
      }
    }
  }

  if (labellast == 1)
    *lastgood = 0;

  strcat(tempchar, "}\n");

  strcpy(cheatcodechar, tempchar);
  free(tempchar);
}

void
getnextcheatline(char* cheatcodechar, int* chtptr, char* chtline)
{
  memset(chtline, 0, MAXCHTLINE * sizeof(char));
  while (1) {
    if ((cheatcodechar[chtptr[0]] == '{') /*|| (cheatcodechar[chtptr[0]]=='}')*/
        || (cheatcodechar[chtptr[0]] ==
            '\n')) { // add code here possibly for label with '{'?
      chtptr[0]++;
      break;
    }
    char thischtchar[] = { 0, 0 };
    thischtchar[0] = cheatcodechar[chtptr[0]];
    if (thischtchar[0] != '\r')
      strcat(chtline, thischtchar);
    chtptr[0]++;
  }
}

void
getnextchtline(char* cheatcodechar, int* chtptr, char* chtline)
{
  memset(chtline, 0, MAXCHTLINE * sizeof(char));

  while (1) {
    char thischtchar;
    thischtchar = cheatcodechar[chtptr[0]];
    *(chtline + strlen(chtline)) = thischtchar;
    chtptr[0]++;
    if (thischtchar == '\n')
      break;
  }
}

void
trim(char* textchar, char* texttotrim, char* replacechar)
{
  char* tempchar = (char*)malloc(MAXCHTLINE * sizeof(char));
  memset(tempchar, 0, MAXCHTLINE * sizeof(char));
  char* lastptr = textchar;
  while (1) {
    char* strptr = strstr(lastptr, texttotrim);
    if (strptr)
      *strptr = 0;
    else {
      strcat(tempchar, lastptr);
      break;
    }
    strcat(tempchar, lastptr);
    strcat(tempchar, replacechar);
    lastptr = strptr + strlen(texttotrim);
  }
  memset(textchar, 0, strlen(tempchar) + 1);
  sprintf(textchar, "%s", tempchar);
  free(tempchar);
}

int
testchtline(char* cheatline)
{
  char goodcharlist[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B',
                          'C', 'D', 'E', 'F', '0', '1', '2', '3',
                          '4', '5', '6', '7', '8', '9', ':', ',' };
  for (int chtptr = 0; chtptr < strlen(cheatline); chtptr++) {
    int thismatch = 0;
    for (int goodcharptr = 0; goodcharptr < strlen(goodcharlist); goodcharptr++)
      if (*(cheatline + chtptr) == *(goodcharlist + goodcharptr))
        thismatch++;
    if (thismatch == 0)
      return 0;
  }
  return 1;
}

int
countcommas(char* textchar)
{
  int commas = 0;
  for (int textptr = 0; textptr < strlen(textchar); textptr++)
    if (*(textchar + textptr) == ',')
      commas++;
  return commas;
}

void
removenpc(char* cheatcodechar)
{
  if (strlen(cheatcodechar) == 0)
    return;
  char* tempccptr = cheatcodechar;
  char* tempchtchar = (char*)malloc(MAXCODELEN);
  memset(tempchtchar, 0, MAXCODELEN);
  char* tempchtptr = tempchtchar;
  for (int chtptr = 0; chtptr < strlen(cheatcodechar); chtptr++) {
    if ((*tempccptr > 0x1F) && (*tempccptr < 0x80) && (*tempccptr != 0)) {
      *tempchtptr = *tempccptr;
      tempchtptr++;
    } else if (*tempccptr == 0xA) {
      *tempchtptr = 0xD;
      *(tempchtptr + 1) = 0xA;
      tempchtptr += 2;
    }
    tempccptr++;
  }
  memset(cheatcodechar, 0, MAXCODELEN);
  if (strlen(tempchtchar) == 0)
    sprintf(cheatcodechar, "Cheat Label\0");
  else
    sprintf(cheatcodechar, "%s\0", tempchtchar);
  free(tempchtchar);
}

void
importcht(char* cheatcodechar)
{

  char* tempchar = (char*)malloc(MAXCODELEN);
  memset(tempchar, 0, MAXCODELEN);

  char* endcht = strstr(cheatcodechar, "GameInfo");
  if (endcht)
    memset(endcht, 0, 8);

  int myptr = 0;
  int descwaslast = 0;

  char* cheatline = (char*)malloc(MAXCHTLINE);
  char* lastdesc = (char*)malloc(MAXCHTLINE);
  char* lastlabel = NULL;
  while (myptr < strlen(cheatcodechar)) {
    getnextchtline(cheatcodechar, &myptr, cheatline);

    if (testcht(cheatline, "OFF=") == 1)
      continue;
    if (strlen(cheatline) > 0) {
      trim(cheatline, "[", "");
      trim(cheatline, "]", "");
      trim(cheatline, ",\r\n", "\r\n");
      trim(cheatline, ";\r\n", "\r\n");
      trim(cheatline, "ON=\r\n", "");
      trim(cheatline, "ON=", "");
      trim(cheatline, "0=", "");
      trim(cheatline, "MAX=", "");
      if (*cheatline == '=')
        trim(cheatline, "=", "");
      trim(cheatline, ";", "\r\n");
      trim(cheatline, "TEXT=", "");
      trim(cheatline, "Text=", "");
      // removenpc(cheatline);
      if (strlen(cheatline) > 1)
        sprintf(tempchar + strlen(tempchar), "%s\r\n", cheatline);
    }
  }

  memset(cheatcodechar, 0, MAXCODELEN);
  memcpy(cheatcodechar, tempchar, strlen(tempchar));
  memset(tempchar, 0, MAXCODELEN);

  myptr = 0;

  while (myptr < strlen(cheatcodechar)) {
    getnextcheatline(cheatcodechar, &myptr, cheatline);
    if (strlen(cheatline) == 0)
      continue;
    if (*cheatline == '=')
      continue;
    char* multimodchar = strstr(cheatline, "=");
    if (multimodchar) {
      if (lastlabel != NULL) {
        *lastlabel = 0;
        lastlabel = NULL;
      }

      *multimodchar = 0;
      sprintf(tempchar + strlen(tempchar), "%s - %s\r\n", lastdesc, cheatline);
      for (int copyptr = 0; copyptr < strlen(multimodchar + 1); copyptr++) {
        *(cheatline + copyptr) = *(multimodchar + 1 + copyptr);
      }
      *(cheatline + strlen(multimodchar + 1)) = 0;
    } else
      descwaslast = 0;

    int cheat = testchtline(cheatline);

    if (cheat == 1) {
      trim(cheatline, ":", ",");
      descwaslast = 0;
      char* addrchar = strstr(cheatline, ",");
      *addrchar = 0;
      addrchar++;
      int addressint = hextoint(cheatline);
      if (addressint == 0x400130) {
        sprintf(tempchar + strlen(tempchar),
                "D0000020 %04X\r\n",
                hextoint(addrchar) ^ 0xff);
        continue;
      }
      if ((addressint == 0) || (addressint > 0x41FFFFF)) {
        sprintf(tempchar + strlen(tempchar),
                "//%08X:%s ;pointer code????\r\n",
                addressint,
                addrchar);
        continue;
      }

      if (addressint <= 0x3FFFF)
        addressint |= 0x2000000;
      else if ((addressint & 0xf000000) == 0)
        addressint = (addressint & 0xffff) | 0x3000000;

      char* morecommas = strstr(addrchar, ",");
      if (!morecommas) {

        switch (strlen(addrchar)) {
          case 1:
          case 2:
            sprintf(tempchar + strlen(tempchar),
                    "3%X %04X\r\n",
                    addressint,
                    hextoint(addrchar));
            break;
          case 3:
          case 4:
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint,
                    hextoint(addrchar));
            break;
          default:
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint,
                    hextoint(addrchar) & 0xffff);
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint + 2,
                    (hextoint(addrchar) >> 16) & 0xffff);
        }

      } else {
        unsigned int values[1024];
        memset(&values, 0, sizeof(int) * 1024);
        int commacount = countcommas(addrchar);
        char* tempaddrchar = addrchar;
        char* newaddrchar;
        int size = 1;
        for (int commaptr = 0; commaptr < commacount + 1; commaptr++) {
          newaddrchar = strstr(tempaddrchar, ",");
          if (newaddrchar)
            *newaddrchar = 0;
          values[commaptr] = hextoint(tempaddrchar);
          if ((values[commaptr] > 0xff) && (size < 2))
            size = 2;
          if (values[commaptr] > 0xffff)
            size = 4;
          tempaddrchar = newaddrchar + 1;
        }

        if (size == 1) {
          switch (commacount) {
            case 1:
              sprintf(tempchar + strlen(tempchar),
                      "8%X %02X%02X\r\n",
                      addressint,
                      values[1],
                      values[0]);
              break;
            case 2:
              if ((addressint % 2) == 0) { // even address to start
                sprintf(tempchar + strlen(tempchar),
                        "8%X %02X%02X\r\n",
                        addressint,
                        values[1],
                        values[0]);
                sprintf(tempchar + strlen(tempchar),
                        "3%X %04X\r\n",
                        addressint + 2,
                        values[2]);
              } else {
                sprintf(tempchar + strlen(tempchar),
                        "3%X %04X\r\n",
                        addressint,
                        values[0]);
                sprintf(tempchar + strlen(tempchar),
                        "3%X %02X%02X\r\n",
                        addressint + 1,
                        values[2],
                        values[1]);
              }
              break;
            case 3:
              sprintf(tempchar + strlen(tempchar),
                      "8%X %02X%02X\r\n",
                      addressint,
                      values[1],
                      values[0]);
              sprintf(tempchar + strlen(tempchar),
                      "8%X %02X%02X\r\n",
                      addressint + 2,
                      values[3],
                      values[2]);
              break;
            default:
              int commahalf = ((int)((commacount + 1) / 2));
              sprintf(tempchar + strlen(tempchar),
                      "5%X %04X\r\n",
                      addressint,
                      commahalf);
              int commaptr = 0;
              while (commaptr < commahalf) {
                int thisval[] = { 0, 0, 0 };
                if (commaptr < commahalf)
                  thisval[0] = ((values[commaptr * 2 + 1]) & 0xff) |
                               ((values[commaptr * 2] & 0xff) << 8);
                if (commaptr + 1 < commahalf)
                  thisval[1] = ((values[commaptr * 2 + 3]) & 0xff) |
                               ((values[commaptr * 2 + 2] & 0xff) << 8);
                if (commaptr + 2 < commahalf)
                  thisval[2] = ((values[commaptr * 2 + 5]) & 0xff) |
                               ((values[commaptr * 2 + 4] & 0xff) << 8);
                sprintf(tempchar + strlen(tempchar),
                        "%04X%04X %04X\r\n",
                        thisval[0],
                        thisval[1],
                        thisval[2]);
                commaptr += 3;
              }
              if ((commacount + 1) % 2 != 0)
                sprintf(tempchar + strlen(tempchar),
                        "3%X %04X\r\n",
                        addressint + commahalf * 2,
                        values[commacount]);
          }
        }
        if (size == 2) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint,
                    values[0]);
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint + 2,
                    values[1]);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar),
                    "5%X %04X\r\n",
                    addressint,
                    commacount + 1);
            for (int commactr = 0; commactr < commacount + 1; commactr += 3) {
              int thisval[] = { 0, 0, 0 };
              if (commactr < commacount + 1)
                thisval[0] = ((values[commactr] >> 8) & 0xff) |
                             ((values[commactr] & 0xff) << 8);
              if (commactr + 1 < commacount + 1)
                thisval[1] = ((values[commactr + 1] >> 8) & 0xff) |
                             ((values[commactr + 1] & 0xff) << 8);
              if (commactr + 2 < commacount + 1)
                thisval[2] = ((values[commactr + 2] >> 8) & 0xff) |
                             ((values[commactr + 2] & 0xff) << 8);
              sprintf(tempchar + strlen(tempchar),
                      "%04X%04X %04X\r\n",
                      thisval[0],
                      thisval[1],
                      thisval[2]);
            }
          }
        }

        if (size == 4) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint,
                    values[0] & 0xffff);
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint + 2,
                    (values[0] >> 16) & 0xffff);
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint + 4,
                    values[1] & 0xffff);
            sprintf(tempchar + strlen(tempchar),
                    "8%X %04X\r\n",
                    addressint + 6,
                    (values[1] >> 16) & 0xffff);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar),
                    "5%X %04X\r\n",
                    addressint,
                    (commacount + 1) * 2);
            for (int commactr = 0; commactr < commacount + 1; commactr++) {
              int thisval = ((values[commactr] & 0xff) << 24) |
                            (((values[commactr] >> 8) & 0xff) << 16) |
                            ((values[commactr] & 0xff0000) >> 8) |
                            ((values[commactr] >> 24) & 0xff);
              if ((commactr % 3) == 0)
                sprintf(tempchar + strlen(tempchar), "%08X ", thisval);
              if ((commactr % 3) == 1)
                sprintf(tempchar + strlen(tempchar),
                        "%04X\r\n%04X",
                        (thisval >> 16) & 0xffff,
                        thisval & 0xffff);
              if ((commactr % 3) == 2)
                sprintf(tempchar + strlen(tempchar),
                        "%04X %04X\r\n",
                        (thisval >> 16) & 0xffff,
                        thisval & 0xffff);
            }
            if (((commacount + 1) % 3) == 1)
              sprintf(tempchar + strlen(tempchar), "0000\r\n");
            if (((commacount + 1) % 3) == 2)
              sprintf(tempchar + strlen(tempchar), "0000 0000\r\n");
          }
        }
      }
    } else {
      descwaslast = 1;
      lastlabel = tempchar + strlen(tempchar);
      sprintf(tempchar + strlen(tempchar), "%s\r\n", cheatline);
      memset(lastdesc, 0, MAXCHTLINE);
      sprintf(lastdesc, "%s", cheatline);
    }
  }
  memset(cheatcodechar, 0, MAXCODELEN);
  memcpy(cheatcodechar, tempchar, MAXCODELEN);

  free(tempchar);
  free(cheatline);

  QTextStream(stdout) << "CHT format detected -- importing cheats";
}

int
convertraw(char* cheatcodes,
           unsigned int* cheatcodeint,
           int wantmenu,
           int cheatselectram,
           unsigned int* menuint)
{

  unsigned int bytewrite[] = {
    0xE3A00000, 0xE59F1004, 0xE5C10000, 0xEA000000, 0xFFFFFFFF
  };
  unsigned int bigwrite[] = { 0xE59F000C, 0xE59F1004, 0xE1C100B0,
                              0xEA000001, 0xFFFFFFFF, 0xEEEEEEEE };

  unsigned int loadramreg[] = { 0xE59FB000, 0xEA000000, cheatselectram };
  unsigned int ramtest[] = {
    0xE5DB0000, 0xE59F1008, 0xE0100001, 0xA000002, 0xEA000000
  };
  int intcounter = 0;
  int cheatcounter = 0;

#define maxaddr 300

  int menujmp = 0;
  int jumpwidth = 0;
  int whichbit = -1;
  char* tempaddr = (char*)malloc(9 * sizeof(char));

  char tempchar[600];

  int asmloop = 0;
  int asmlabel = 0;
  int labellast = 0;
  int* cheatptr = (int*)malloc(2 * sizeof(int));
  char* cheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  char* lastcheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  cheatptr[0] = 0;
  cheatline[0] = 0;

  if (wantmenu == 1) {
    copyint(cheatcodeint + intcounter, loadramreg, 3);
    intcounter += 3;
  }

  while (cheatptr[0] < strlen(cheatcodes)) {
    if (strlen(cheatline) > 0) {
      strcpy(lastcheatline, cheatline);
    }
    getnextcheatline(cheatcodes, cheatptr, cheatline);

    if ((cheatline[0] == '[') && (wantmenu == 1)) {
      int menuoffset = cheatcounter * 7 + 1;
      int menuitemlength = strstr(cheatline, "]") - cheatline - 2;
      if (menuitemlength > 24)
        menuitemlength = 24;
      memcpy(menuint + menuoffset, cheatline + 1, menuitemlength);
    }
    if (cheatline[0] == '[') {
      cheatcounter++;
      if (wantmenu == 1) {
        whichbit++;
        if (whichbit == 8) {
          *(cheatcodeint + intcounter) = 0xE28BB001;
          intcounter++;
          whichbit = 0;
        }
      }
    }

    if ((cheatline[0] == '}') && (wantmenu == 1)) {
      *menuint = 0x1c00 | cheatcounter;
      *(cheatcodeint + menujmp) += jumpwidth;
      menujmp = 0;
      jumpwidth = 0;
    }

    if ((cheatline[0] == '[') || (cheatline[0] == '{') ||
        (cheatline[0] == '}')) {
      // add some labeling code here if needed
      cheatline[0] = 0;
    }
    if (strlen(cheatline) != 0) {

      char templongchar[9];
      memset(templongchar, 0, 9);
      strright(cheatline, templongchar, 8);

      int decval = hextoint(templongchar);

      strleft(cheatline, tempaddr, 8);

      int tempdec = hextoint(tempaddr);
      sprintf(tempaddr, "%X", (tempdec & 0xfffffff));

      if (decval <= 0xff) {

        if (wantmenu == 1) {
          copyint(cheatcodeint + intcounter, ramtest, 5);
          *(cheatcodeint + intcounter + 5) = (1 << whichbit);
          if (menujmp == 0) {
            menujmp = intcounter + 3;
            jumpwidth = 4;
          } else {
            jumpwidth += 5;
          }
          intcounter += 6;
        }

        copyint(cheatcodeint + intcounter, bytewrite, 5);
        *(cheatcodeint + intcounter) |= decval;
        *(cheatcodeint + intcounter + 4) = tempdec;
        intcounter += 5;

      } else {

        if (wantmenu == 1) {
          copyint(cheatcodeint + intcounter, ramtest, 5);
          *(cheatcodeint + intcounter + 5) = (1 << whichbit);
          if (menujmp == 0) {
            menujmp = intcounter + 2;
            jumpwidth = 5;
          } else {
            jumpwidth += 6;
          }
          intcounter += 3;
        }

        copyint(cheatcodeint + intcounter, bigwrite, 6);
        *(cheatcodeint + intcounter + 4) = tempdec;
        *(cheatcodeint + intcounter + 5) = decval;

        if (decval > 0xffff)
          *(cheatcodeint + intcounter + 2) = 0xE5810000;

        intcounter += 6;
      }
    }
  }

  *(cheatcodeint + intcounter) = 0xE12FFF1E;
  intcounter++;

  *(menuint + cheatcounter * 7 + 1) = 0xEFBEADDE;

  return intcounter;
}

int
convertcb(char* cheatcodes,
          unsigned int* cheatcodeint,
          int wantmenu,
          int cheatselectram,
          unsigned int* menuint)
{

  unsigned int iftopbyte[] = { 0xE3A02000, 0xE59F100C, 0xE1D100B0 };
  unsigned int superint[] = { 0xE59F1014, 0xE28F2014, 0xE0D200B2, 0xE0C100B2,
                              0xE2533001, 0x1AFFFFFB, 0xEA000000 };
  unsigned int slideint[] = { 0xE1DF21BE, 0xE1DF31BC, 0xE1DF41BA, 0xE08100B2,
                              0xE0800004, 0xE2533001, 0x1AFFFFFB, 0xEA000002 };
  unsigned int dpadint[] = { 0xE3A01301, 0xE5911130, 0xE3A00C03, 0xE28000FF,
                             0xE0010000, 0xE1500002, 0x1A000000 };

  unsigned int loadramreg[] = { 0xE59FB000, 0xEA000000, cheatselectram };
  unsigned int ramtest[] = {
    0xE5DB0000, 0xE59F1008, 0xE0100001, 0xA000000, 0xEA000000
  };

  int intcounter = 0;
  int menujumppatch = 0;
  int jumppatches[10];
  int numpatches = 0;
  int superptr = 0;
  int slideval = -1;
  int whichbit = -1;
  int cheatcounter = 0;
  int mastercodeon = 0;

#define maxaddr 300

  char* tempaddr = (char*)malloc(9 * sizeof(char));
  char* tempstr = (char*)malloc(50 * sizeof(char));
  char* lvalstr = (char*)malloc(4 * sizeof(char));
  char* templongchar = (char*)malloc(9 * sizeof(char));

  int labellevel = 0;
  int encryptionon = 0;
  int slideon = 0;
  int superon = 0;
  int conditionalon = 0;
  int conditionallast = 0;
  int nextconditional = 0;
  char tempchar[600];

  int labellast = 1; //=0;
  int* cheatptr = (int*)malloc(2 * sizeof(int));
  char* cheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  char* lastcheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  cheatptr[0] = 0;
  cheatline[0] = 0;

  if (wantmenu == 1) {
    copyint(cheatcodeint + intcounter, loadramreg, 3);
    intcounter += 3;
  }

  while (cheatptr[0] < strlen(cheatcodes)) {
    if (strlen(cheatline) > 0) {
      strcpy(lastcheatline, cheatline);
    }
    getnextcheatline(cheatcodes, cheatptr, cheatline);

    if (cheatline[0] == '[') {

      labellast = 1;

      if (wantmenu == 1) {
        whichbit++;
        if (whichbit == 8) {
          *(cheatcodeint + intcounter) = 0xE28BB001;
          intcounter++;
          whichbit = 0;
        }

        menujumppatch = intcounter + 3;

        int menuoffset = cheatcounter * 7 + 1;
        int menuitemlength = strstr(cheatline, "]") - cheatline - 1; // 2;
        if (menuitemlength > 24)
          menuitemlength = 24;
        memcpy(menuint + menuoffset, cheatline + 1, menuitemlength);

        copyint(cheatcodeint + intcounter, ramtest, 5);
        *(cheatcodeint + intcounter + 5) = (1 << whichbit);
        intcounter += 6;
        conditionalon = 2;
        conditionallast = 1;

      } else
        menujumppatch = 0;

      cheatcounter++;
    }

    if ((cheatline[0] == '}') && (wantmenu == 1)) {
      *menuint = 0x1c00 | cheatcounter;
      *(cheatcodeint + menujumppatch) |= (intcounter - menujumppatch - 2);
    }

    nextconditional = 0;

    switch (cheatline[0]) {
      case '7':
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'F':

        if ((conditionallast == 0) && (labellast == 0) && (encryptionon == 0)) {
          conditionalon = 2; // change to 2?
          nextconditional = 1;
        }
        break;
      default:
        break;
    }

    if ((cheatline[0] == '}') || (nextconditional == 1)) {
      if (conditionalon != 0) {
        labellast = 1;
        conditionalon = 0;
        nextconditional = 0;

        for (int jumpctr = 0; jumpctr < numpatches; jumpctr++) {
          *(cheatcodeint + jumppatches[jumpctr]) |=
            (intcounter - jumppatches[jumpctr] - 2);
        }

        numpatches = 0;
      }
    }

    conditionallast = 0;

    if ((cheatline[0] == '[') || (cheatline[0] == '{')) {
      labellast = 1; // might need to take out

      // add some labeling code here if needed
      cheatline[0] = 0;
      mastercodeon = 0;
    } else
      labellast = 0;

    if (mastercodeon == 1)
      continue; // cheatline[0]=0;

    if (superon <= 0) {
      if ((((cheatline[0] == '0') && (slideon == 0)) ||
           (cheatline[0] == '1')) &&
          (encryptionon == 0)) {
        intcounter -= 6;
        conditionalon = 0;
        whichbit--;
        mastercodeon = 1;
        cheatcounter--;
        cheatline[0] = 0;
      }
    }

    if (strlen(cheatline) != 0) {
      if (encryptionon == 1) {
        char tempaddressstr[9];
        memset(tempaddressstr, 0, 9);
        memcpy(tempaddressstr, cheatline, 8);
        int tempaddress = hextoint(tempaddressstr);
        char tempvaluestr[9];
        memset(tempvaluestr, 0, 9);
        memcpy(tempvaluestr, cheatline + 9, 8);
        int tempvalue = hextoint(tempvaluestr);
        unsigned char codebuffer[8] = { (tempaddress & 255),
                                        ((tempaddress >> 8) & 255),
                                        ((tempaddress >> 16) & 255),
                                        ((tempaddress >> 24) & 255),
                                        (tempvalue & 255),
                                        ((tempvalue >> 8) & 255),
                                        0,
                                        0 };
        cheatsCBADecrypt(codebuffer);
        int* codebuffint = (int*)codebuffer;
        tempaddress = *codebuffint;
        short* codebuffshort = (short*)codebuffer;
        tempvalue = *(codebuffint + 1);
        sprintf(cheatline, "%08X %08X", tempaddress, tempvalue);
      }

      //	* CodeBreaker codes types: (based on the CBA clone "Cheatcode S"
      // v1.1)
      //	*
      //	* 0000AAAA 000Y - Game CRC (Y are flags: 8 - CRC, 2 - DI)
      //	* 1AAAAAAA YYYY - Master Code function (store address at ((YYYY
      //<< 0x16)
      //	*                 + 0x08000100))
      //	* 2AAAAAAA YYYY - 16-bit or
      //	* 3AAAAAAA YYYY - 8-bit constant write
      //	* 4AAAAAAA YYYY - Slide code
      //	* XXXXCCCC IIII   (C is count and I is address increment, X is
      // value incr.)
      //	* 5AAAAAAA CCCC - Super code (Write bytes to address, 2*CCCC is
      // count)
      //	* BBBBBBBB BBBB
      //	* 6AAAAAAA YYYY - 16-bit and
      //	* 7AAAAAAA YYYY - if address contains 16-bit value enable next
      // code
      //	* 8AAAAAAA YYYY - 16-bit constant write
      //	* 9AAAAAAA YYYY - change decryption (when first code only?)
      //	* AAAAAAAA YYYY - if address does not contain 16-bit value
      // enable next code
      //	* BAAAAAAA YYYY - if 16-bit value at address  <= YYYY skip next
      // code
      //	* CAAAAAAA YYYY - if 16-bit value at address  >= YYYY skip next
      // code
      //	* D00000X0 YYYY - if button keys ... enable next code (else skip
      // next code)
      //	* EAAAAAAA YYYY - increase 16/32bit value stored in address
      //	* FAAAAAAA YYYY - if 16-bit value at address AND YYYY = 0 then
      // skip next code
      memset(tempaddr, 0, 9);
      strleft(cheatline, tempaddr, 8);
      int tempdec = hextoint(tempaddr);

      memset(templongchar, 0, 9);
      strright(cheatline, templongchar, 8);
      int decval = hextoint(templongchar) & 0xffff;

      if (superon > 0) {

        if ((superon % 4) == 0) { // if (((superon/6)%2)==0) {
          *(cheatcodeint + intcounter) = byteflipint(tempdec);
          intcounter++;

          if (superon > 4) {
            *(cheatcodeint + intcounter) =
              (((decval & 0xff00) >> 8) | ((decval & 0xff) << 8));
            intcounter++;
          }
        } else {
          *(cheatcodeint + intcounter - 1) = byteflipint(
            ((((*(cheatcodeint + intcounter - 1) & 0xff) << 8) |
              ((*(cheatcodeint + intcounter - 1) & 0xff00) >> 8))
             << 16) |
            ((tempdec & 0xffff0000) >> 16)); // byteflipint(>>16)0x7788<<16

          if (superon > 4) {
            *(cheatcodeint + intcounter) =
              byteflipint(((decval & 0xffff)) | ((tempdec & 0xffff) << 16));
            // HexMessage(*(cheatcodeint+intcounter), "byte flipped value");
            intcounter++;
          }
        }

        superon -= 6; // 3;

        *(cheatcodeint + superptr) = 0xea000000 | (intcounter - superptr - 2);
        // if (superon==0) conditionalon--;
        if (superon > 0) {
        } else {

          labellast = 1;
        }
        continue;
      }

      if (slideon == 1) {

        copyint((cheatcodeint + intcounter + 2), slideint, 8);
        *(cheatcodeint + intcounter + 9) = 0xEA000002;
        *(cheatcodeint + intcounter + 11) = ((decval << 16) | slideval);
        *(cheatcodeint + intcounter + 12) = tempdec;
        intcounter += 13;

        cheatline[0] = 0;
        slideon = 0;
      }

      if (cheatline[0] == '4') {

        *(cheatcodeint + intcounter) = 0xE1DF02B4;
        slideval = decval;

        *(cheatcodeint + intcounter + 1) = 0xE59F101C;
        *(cheatcodeint + intcounter + 10) = (tempdec & 0xfffffff);

        slideon = 1;
        cheatline[0] = 0;
        continue;
      }

      if (cheatline[0] == '5') {
        if (decval <= 0xff) {
          *(cheatcodeint + intcounter) = (0xE3A03000 | decval);
        } else {
          *(cheatcodeint + intcounter) = 0xE59F3018;
        }

        superon = decval * 2;
        copyint(cheatcodeint + intcounter + 1, superint, 7);
        *(cheatcodeint + intcounter + 8) = (tempdec & 0xfffffff);
        superptr = intcounter + 7; // 7;

        if (decval <= 0xff) {
          intcounter += 9; // 9;
        } else {
          *(cheatcodeint + intcounter + 9) = (decval & 0xffff);
          intcounter += 10; // 10;
          superptr++;
        }

        cheatline[0] = 0;
        continue;
      }

      if ((cheatline[0] == '9') && (encryptionon == 0)) {
        unsigned int seed[8];
        cheatsCBAParseSeedCode(tempdec, decval, seed);
        cheatsCBAChangeEncryption(seed);
        encryptionon = 1;
        mastercodeon = 1;
        conditionalon = 0;
        whichbit--;
        cheatcounter--;
        cheatline[0] = 0;
      }

      if ((cheatline[0] == '2') || (cheatline[0] == '6') ||
          (cheatline[0] == 'E')) {

        if (decval <= 0xff) {

          *(cheatcodeint + intcounter) = (0xE3A02000 | decval);

        } else {
          *(cheatcodeint + intcounter) = 0xE59F2014;
        }

        *(cheatcodeint + intcounter + 1) = 0xE59F100C;
        *(cheatcodeint + intcounter + 2) = 0xE1D100B0;

        if (cheatline[0] == '2') {
          *(cheatcodeint + intcounter + 3) = 0xE1800002;
        }
        if (cheatline[0] == '6') {
          *(cheatcodeint + intcounter + 3) = 0xE0000002;
        }
        if (cheatline[0] == 'E') {
          *(cheatcodeint + intcounter + 3) = 0xE0800002;
        }

        *(cheatcodeint + intcounter + 4) = 0xE1C100B0;
        if (decval <= 0xff) {
          *(cheatcodeint + intcounter + 5) = 0xEA000000;
        } else {
          *(cheatcodeint + intcounter + 5) = 0xEA000001;
        }

        *(cheatcodeint + intcounter + 6) = (tempdec & 0xfffffff);

        intcounter += 7;

        if (decval > 0xff) {
          *(cheatcodeint + intcounter) = decval;
          intcounter++;
        }
      }

      if ((cheatline[0] == '3') || (cheatline[0] == '8')) {

        if ((cheatline[0] == '3') || (decval <= 0xff)) {

          *(cheatcodeint + intcounter) = (0xE3A00000 | decval);

        } else {

          *(cheatcodeint + intcounter) = 0xE59F000C;
        }

        *(cheatcodeint + intcounter + 1) = 0xE59F1004;

        if (cheatline[0] == '3') {

          *(cheatcodeint + intcounter + 2) = 0xE5C10000;
          *(cheatcodeint + intcounter + 3) = 0xEA000000;
          *(cheatcodeint + intcounter + 4) = (tempdec & 0xfffffff);

          intcounter += 5;

        } else {

          *(cheatcodeint + intcounter + 2) = 0xE1C100B0;
          *(cheatcodeint + intcounter + 3) = 0xEA000001;
          *(cheatcodeint + intcounter + 4) = (tempdec & 0xfffffff);
          *(cheatcodeint + intcounter + 5) = (decval & 0xffff);

          intcounter += 6;
        }
      }

      if ((cheatline[0] == '7') || (cheatline[0] == 'A') ||
          (cheatline[0] == 'B') || (cheatline[0] == 'C') ||
          (cheatline[0] == 'F')) {

        if (decval <= 0xff) {

          copyint(cheatcodeint + intcounter, iftopbyte, 3);
          *(cheatcodeint + intcounter) |= decval;

        } else {

          copyint(cheatcodeint + intcounter, iftopbyte, 3);
          *(cheatcodeint + intcounter) = 0xE59F2014;
        }

        if (cheatline[0] == 'F') {
          if (decval > 0xff) {
            *(cheatcodeint + intcounter) += 4;
          }
          *(cheatcodeint + intcounter + 1) += 4;
        }

        intcounter += 3;

        if (cheatline[0] == 'F') {
          *(cheatcodeint + intcounter) = 0xE0000002;
          intcounter++;
        }

        *(cheatcodeint + intcounter) = 0xE1500002;
        intcounter++;

        // bne = 1a, beq = 0a, ble = da, bge = aa

        if (cheatline[0] == '7') {

          *(cheatcodeint + intcounter) = 0x1a000000;
        }
        if (cheatline[0] == 'A') {

          *(cheatcodeint + intcounter) = 0x0a000000;
        }
        if (cheatline[0] == 'B') {

          *(cheatcodeint + intcounter) = 0xda000000;
        }
        if (cheatline[0] == 'C') {

          *(cheatcodeint + intcounter) = 0xaa000000;
        }
        if (cheatline[0] == 'F') { // might need to fix

          if (decval > 0xff) {
            *(cheatcodeint + intcounter - 4) += 4;
          }
          *(cheatcodeint + intcounter - 3) += 4;
          *(cheatcodeint + intcounter + 1) = 0x1a000000;
          intcounter++;
        }

        jumppatches[numpatches] = intcounter;
        if (cheatline[0] == 'F')
          jumppatches[numpatches]++;
        numpatches++;

        intcounter++;

        *(cheatcodeint + intcounter) = 0xEA000000;
        if (decval > 0xff)
          *(cheatcodeint + intcounter) |= 1;

        intcounter++;

        *(cheatcodeint + intcounter) = (tempdec & 0xfffffff);

        intcounter++;

        if (decval > 0xff) {
          *(cheatcodeint + intcounter) = decval;
          intcounter++;
        }

        conditionalon = 2;
        conditionallast = 1;
      }

      if (cheatline[0] == 'D') {
        decval ^= 0x3ff;
        if (decval <= 0xff) {
          *(cheatcodeint + intcounter) = (0xE3A02000 | decval);
        } else {

          *(cheatcodeint + intcounter) = 0xE59F201C; // 7 0; //4;
        }

        copyint(cheatcodeint + intcounter + 1, dpadint, 7); // 5);
        intcounter += 8;
        jumppatches[numpatches] = intcounter - 1;
        numpatches++;

        if (decval > 0xff) {
          *(cheatcodeint + intcounter) = 0xEA000000;
          *(cheatcodeint + intcounter + 1) = decval;
          intcounter += 2;
        }

        conditionalon = 2;
        conditionallast = 1;
      }

      switch (cheatline[0]) {
        case '7':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'F':
          break;
        default:
          conditionallast = 0;
          break;
      }
    }
  }

  *(cheatcodeint + intcounter) = 0xE12FFF1E;
  intcounter++;

  *(menuint + cheatcounter * 7 + 1) = 0xEFBEADDE;
  return intcounter;
}

/*
int convertgs(char * cheatcodes,unsigned int * cheatcodeint) {

int intcounter=0;

#define maxaddr 300

    char * asmtop=(char *)malloc(20480*sizeof(char));
    char * asmaddresses=(char *)malloc(4096*sizeof(char));
    //char * asmhvals=(char *)malloc(1024*sizeof(char));
    char * asmlvals=(char *)malloc(4096*sizeof(char));
    //char * asmdata=(char *)malloc(1024*sizeof(char));
    char * addrstr=(char *)malloc(4*sizeof(char)); //change back to 3?
    char * tempaddr=(char *)malloc(9*sizeof(char));
    char * tempstr=(char *)malloc(50*sizeof(char));
    char * lvalstr=(char *)malloc(4*sizeof(char)); //change back to 3?
    //char * hvalstr=(char *)malloc(3*sizeof(char));
    char * templongchar=(char *)malloc(9*sizeof(char));

    int labellevel=0;
    int labeladdr[30];
    int encryptionon=0;
    int slideon=0;
    int superon=0;
    int conditionalon=0;
    char tempchar[600];

    ADDRESSSTRUCT myaddressstruct;
    LVALSTRUCT mylvalstruct;
    myaddressstruct.asmaddr=(unsigned int *)malloc(1*sizeof(unsigned int));
    *myaddressstruct.asmaddr=0;
    mylvalstruct.asmlvalue=(unsigned int *)malloc(1*sizeof(unsigned int));
    *mylvalstruct.asmlvalue=0;
    myaddressstruct.oldasmaddrs=(unsigned int *)malloc(maxaddr*sizeof(unsigned
int)); mylvalstruct.oldasmlvalues=(unsigned int *)malloc(maxaddr*sizeof(unsigned
int)); unsigned int * myaddrs=(unsigned int *)malloc(maxaddr*sizeof(unsigned
int)); myaddressstruct.oldasmaddrs=myaddrs; unsigned int * mylvals=(unsigned int
*)malloc(maxaddr*sizeof(unsigned int)); mylvalstruct.oldasmlvalues=mylvals; for
(int fillloop=0; fillloop<maxaddr; fillloop++) {
        *(myaddressstruct.oldasmaddrs+fillloop)=0x0;
        *(mylvalstruct.oldasmlvalues+fillloop)=0x0;
    }
    char * asmlabelstr=(char *)malloc(3*sizeof(char));
    //char * asmvaluestr=(char *)malloc(3*sizeof(char));

    memset(asmtop,0,20480*sizeof(char));
    memset(asmaddresses,0,4096*sizeof(char));
    //memset(asmhvals,0,1024*sizeof(char));
    memset(asmlvals,0,4096*sizeof(char));
    //memset(asmdata,0,1024*sizeof(char));

    int asmloop=0;
    int asmlabel=0;
    int labellast=0;
    int * cheatptr=(int *)malloc(2*sizeof(int));
    char * cheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
    char * lastcheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
    cheatptr[0]=0;
    cheatline[0]=0;

    while (cheatptr[0]<strlen(cheatcodes)) {
        if (strlen(cheatline)>0) { strcpy(lastcheatline,cheatline); }
        getnextcheatline(cheatcodes,cheatptr,cheatline);


        if ((cheatline[0]=='[') || (cheatline[0]=='{') || (cheatline[0]=='}')) {
            //add some labeling code here if needed
            if ((cheatline[0]=='{') && (labellast==0)) {
                sprintf(asmtop+strlen(asmtop),"label%d:\n",asmlabel);
                asmlabel++;
                labellast=1;
            }
            cheatline[0]=0;
        }
        else {
            labellast=0;
        }

        if (strlen(cheatline)!=0) {
            if (encryptionon==1) {
                //ErrorMessage(cheatline);
                //decrypt code
                char tempaddressstr[9];
                memset(tempaddressstr,0,9);
                memcpy(tempaddressstr,cheatline,8);
                int tempaddress=hextoint(tempaddressstr);
                //HexMessage(tempaddress,tempaddressstr);
                char tempvaluestr[9];
                memset(tempvaluestr,0,9);
                //strright(tempvaluestr,cheatline,4);
                memcpy(tempvaluestr,cheatline+9,8);
                int tempvalue=hextoint(tempvaluestr);
                //HexMessage(tempvalue,tempvaluestr);
                unsigned char
codebuffer[8]={(tempaddress&255),((tempaddress>>8)&255),((tempaddress>>16)&255),((tempaddress>>24)&255),(tempvalue&255),((tempvalue>>8)&255),0,0};
                cheatsCBADecrypt(codebuffer);
                int * codebuffint=(int *)codebuffer;
                tempaddress=*codebuffint;
                short * codebuffshort=(short *)codebuffer;
                tempvalue=*(codebuffint+1);
                sprintf(cheatline,"%X 0000%X",tempaddress,tempvalue);
                //ErrorMessage(cheatline);
            }


        //	* CodeBreaker codes types: (based on the CBA clone "Cheatcode S"
v1.1)
        //	*
        //	* 0000AAAA 000Y - Game CRC (Y are flags: 8 - CRC, 2 - DI)
        //	* 1AAAAAAA YYYY - Master Code function (store address at ((YYYY
<< 0x16)
        //	*                 + 0x08000100))
        //	* 2AAAAAAA YYYY - 16-bit or
        //	* 3AAAAAAA YYYY - 8-bit constant write
        //	* 4AAAAAAA YYYY - Slide code
        //	* XXXXCCCC IIII   (C is count and I is address increment, X is
value incr.)
        //	* 5AAAAAAA CCCC - Super code (Write bytes to address, 2*CCCC is
count)
        //	* BBBBBBBB BBBB
        //	* 6AAAAAAA YYYY - 16-bit and
        //	* 7AAAAAAA YYYY - if address contains 16-bit value enable next
code
        //	* 8AAAAAAA YYYY - 16-bit constant write
        //	* 9AAAAAAA YYYY - change decryption (when first code only?)
        //	* AAAAAAAA YYYY - if address does not contain 16-bit value
enable next code
        //	* BAAAAAAA YYYY - if 16-bit value at address  <= YYYY skip next
code
        //	* CAAAAAAA YYYY - if 16-bit value at address  >= YYYY skip next
code
        //	* D00000X0 YYYY - if button keys ... enable next code (else skip
next code)
        //	* EAAAAAAA YYYY - increase 16/32bit value stored in address
        //	* FAAAAAAA YYYY - if 16-bit value at address AND YYYY = 0 then
skip next code memset(tempaddr,0,9); strleft(cheatline,tempaddr,8); unsigned int
tempdec=hextoint(tempaddr);

                memset(templongchar,0,9);
                strright(cheatline,templongchar,8);
                unsigned int decval=hextoint(templongchar);

                unsigned int * decryptaddr=(unsigned int *)malloc(sizeof(int));
                unsigned int * decryptval=(unsigned int *)malloc(sizeof(int));
                *decryptaddr=tempdec;
                *decryptval=decval;
                GSdecrypt(decryptaddr,decryptval);
                printf(tempchar,"%X %X",*decryptaddr,*decryptval);
                //ErrorMessage(tempchar);
                return 0;


                if (superon>0) {
                    superon-=6;
                    sprintf(asmtop+strlen(asmtop),"0x%X, 0x%X,
0x%X",byteflip((tempdec>>16)&0xffff)&0xffff,byteflip(tempdec&0xffff)&0xffff,byteflip(decval)&0xffff);
                    if (superon>0) {
                        sprintf(asmtop+strlen(asmtop),", ");
                    }
                    else {
                        sprintf(asmtop+strlen(asmtop),"\nlabel%d:\n",asmlabel);
                        asmlabel++;
                        labellast=1;
                    }
                    cheatline[0]=0;
                }

            if (slideon==1) {

                for (int i=2; i<5; i++) {
                    if (i==3) { decval=tempdec&0xffff; }
                    if (i==4) { decval=(tempdec>>16)&0xffff; }
                if (decval<=0xff) {
                    sprintf(asmtop+strlen(asmtop),"mov r%d,#0x%X\n",i,decval);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr
r%d,lval%d\n",i,thislval);
                }
                }
                sprintf(asmtop+strlen(asmtop),"loop%d:\nstrh r0,[r1],r2\nadd
r0,r0,r4\nsubs r3,r3,#0x1\nbne loop%d\n",asmloop,asmloop); asmloop++;
                cheatline[0]=0;
                slideon=0;
                conditionalon--;
            }

            if (cheatline[0]=='4') {
                if (decval<=0xff) {
                    sprintf(asmtop+strlen(asmtop),"mov r0,#0x%X\n",decval);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr r0,lval%d\n",thislval);
                }
                sprintf(tempaddr,"%X",(tempdec&0xfffffff));
                int thisaddr=addresstest(tempaddr,asmaddresses,myaddressstruct);
                sprintf(asmtop+strlen(asmtop),"ldr r1,address%d\n",thisaddr);
                slideon=1;
            }

            if (cheatline[0]=='5') {
                if (decval<=0xff) {
                    sprintf(asmtop+strlen(asmtop),"mov r3,#0x%X\n",decval);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr r3,lval%d\n",thislval);
                }
                sprintf(tempaddr,"%X",(tempdec&0xfffffff));
                int thisaddr=addresstest(tempaddr,asmaddresses,myaddressstruct);
                sprintf(asmtop+strlen(asmtop),"ldr r1,address%d\nadd
r2,r15,#0x10\nloop%d:\nldrh r0,[r2],#0x2\nstrh r0,[r1],#0x2\nsubs
r3,r3,#0x1\nbne loop%d\nb label%d\n.short ",thisaddr,asmloop,asmloop,asmlabel);
                asmloop++;
                superon=decval*2;
            }

            if (cheatline[0]=='9') {
                unsigned int seed[8];
                cheatsCBAParseSeedCode(tempdec,decval,seed);
                cheatsCBAChangeEncryption(seed);
                encryptionon=1;
            }


            if ((cheatline[0]=='2') || (cheatline[0]=='6') ||
(cheatline[0]=='E')) {

                if (decval<=0xff) {
                sprintf(asmtop+strlen(asmtop),"mov r2,#0x%X\n",decval);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr r2,lval%d\n",thislval);
                }

                sprintf(tempaddr,"%X",(tempdec&0xfffffff));
                int thisaddr=addresstest(tempaddr,asmaddresses,myaddressstruct);
                sprintf(asmtop+strlen(asmtop),"ldr r1,address%d\nldrh
r0,[r1]\n",thisaddr);

                if (cheatline[0]=='2') {
                    sprintf(asmtop+strlen(asmtop),"orr r0,r0,r2\n");
                }
                if (cheatline[0]=='6') {
                    sprintf(asmtop+strlen(asmtop),"and r0,r0,r2\n");
                }
                if (cheatline[0]=='E') {
                    sprintf(asmtop+strlen(asmtop),"add r0,r0,r2\n");
                }

                sprintf(asmtop+strlen(asmtop),"strh r0,[r1]\n");
                conditionalon--;

            }

            if ((cheatline[0]=='3') || (cheatline[0]=='8')) {

                sprintf(tempaddr,"%X",(tempdec&0xfffffff));
                int thisaddr=addresstest(tempaddr,asmaddresses,myaddressstruct);

                if ((cheatline[0]=='3') || (decval<=0xff)) {
                    sprintf(asmtop+strlen(asmtop),"mov r0,#0x%X\n",decval&0xff);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr r0,lval%d\n",thislval);
                }

                if (cheatline[0]=='3') {
                    sprintf(asmtop+strlen(asmtop),"ldr r1,address%d\nstrb
r0,[r1]\n",thisaddr);
                }
                else {
                    sprintf(asmtop+strlen(asmtop),"ldr r1,address%d\nstrh
r0,[r1]\n",thisaddr);
                }
                conditionalon--;

            }

            if ((cheatline[0]=='7') || (cheatline[0]=='A') ||
(cheatline[0]=='B') || (cheatline[0]=='C') || (cheatline[0]=='F')) {
                sprintf(tempaddr,"%X",(tempdec&0xfffffff));
                int thisaddr=addresstest(tempaddr,asmaddresses,myaddressstruct);

                if (decval<=0xff) {
                    sprintf(asmtop+strlen(asmtop),"mov r2,#0x%X\nldr
r1,address%d\nldrh r0,[r1]\n",decval,thisaddr);
                }
                else {
                    sprintf(templongchar,"%X",decval);
                    int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    sprintf(asmtop+strlen(asmtop),"ldr r2,lval%d\nldr
r1,address%d\nldrh r0,[r1]\n",thislval,thisaddr);
                }

                if (cheatline[0]=='7') {
                    //sprintf(asmtop+strlen(asmtop),"cmp r0,r2\nbne
label%d\n",asmlabel);
                }
                if (cheatline[0]=='A') {
                    //sprintf(asmtop+strlen(asmtop),"cmp r0,r2\nbeq
label%d\n",asmlabel);
                }
                if (cheatline[0]=='B') {
                    //sprintf(asmtop+strlen(asmtop),"cmp r0,r2\nble
label%d\n",asmlabel);
                }
                if (cheatline[0]=='C') {
                    //sprintf(asmtop+strlen(asmtop),"cmp r0,r2\nbge
label%d\n",asmlabel);
                }
                if (cheatline[0]=='F') { //might need to fix
                    //sprintf(asmtop+strlen(asmtop),"and r0,r0,r2\ncmp
r0,r2\nbne label%d\n",asmlabel);
                }
                conditionalon=2;
            }


            if (cheatline[0]=='D') {
                if (decval<=0xff) {
                //sprintf(asmtop+strlen(asmtop),"mov r2,#0x%X\n",decval);
                }
                else {
                    //sprintf(templongchar,"%X",decval);
                    //int
thislval=longvaluetest(templongchar,asmlvals,mylvalstruct);
                    //sprintf(asmtop+strlen(asmtop),"ldr r2,lval%d\n",thislval);
                }//
                //sprintf(asmtop+strlen(asmtop),"mov r1,#0x4000000\nldr
r0,[r1,#+0x130]\nand r0,r0,r2\ncmp r0,r2\nbeq label%d\n",asmlabel);
                conditionalon=2;
            }

            if (conditionalon==1) {
                //sprintf(asmtop+strlen(asmtop),"label%d:\n",asmlabel);
                //asmlabel++;
                //labellast=1;
                conditionalon--;
            }



        }
        }


        //sprintf(asmtop+strlen(asmtop),"bx r14\n"); //"b return\n");
//"label%d:\nb return\n",asmlabel);
        //memset(cheatcodes,0,20480);
        //sprintf(cheatcodes,"%s\n",asmtop);
        //strcat(cheatcodes,asmaddresses);
        //strcat(cheatcodes,asmlvals);
        //strcat(cheatcodes,"return:\n\n");


        //*(menuint+cheatcounter*7+1)=0xEFBEADDE;
        return intcounter;
}
*/
