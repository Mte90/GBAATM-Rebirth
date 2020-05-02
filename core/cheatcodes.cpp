#include "../functions.h"
#include "../variables.h"

int
hextoint(char* hexstr)
{
  int pos = 0;
  char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'a', 'b', 'c', 'd', 'e', 'f' };
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
        crc = (((crc << 0x08) ^ cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++]) << 0x10) >> 0x10;
        crc = (((crc << 0x08) ^ cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++]) << 0x10) >> 0x10;
        crc = (((crc << 0x08) ^ cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++]) << 0x10) >> 0x10;
        crc = (((crc << 0x08) ^ cheatsCBATable[(((u32)crc << 0x10) >> 0x18) ^ *rom++]) << 0x10) >> 0x10;
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
  *((u16*)(decrypt + 4)) = (cheatsCBAGetData(decrypt) ^ cheatsCBASeed[1]) & 0xffff;

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
  *((u16*)(decrypt + 4)) = (cheatsCBAGetData(decrypt) ^ cheatsCBASeed[3]) & 0xffff;
}

void
GSdecrypt(unsigned int* addressptr, unsigned int* valueptr)
{
  unsigned int address = *addressptr;
  unsigned int value = *valueptr;
  unsigned int seed = 0xC6EF3720;
  int decrypt = 32;
  while (decrypt > 0) {
    value -= ((((address << 4) + seed2) ^ (address + seed)) ^ ((address >> 5) + seed3));
    address -= ((((value << 4) + seed0) ^ (value + seed)) ^ ((value >> 5) + seed1));
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
  return (((number >> 24) & 0xff) | ((number >> 8) & 0xff00) | ((number << 8) & 0xff0000) | ((number << 24) & 0xff000000));
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
  for (int charptr = 0; charptr < (int)strlen(cheatcodechar); charptr++) {
    thischar = *(cheatcodechar + charptr);
    if ((thischar >= 0x61) && (thischar <= 0x7a))
      thischar -= 0x20;
    *(tempchar + charptr) = thischar;
  }

  for (charptr = 0; charptr < (int)strlen(srchstr); charptr++) {
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

char*
formatcheats(char* cheatcodechar)
{
  char goodcodechar[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', ' ' };
  char lastchar[] = { 0, 0 };
  char thischar[] = { 0, 0 };
  int labellast = 0;
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

    if (((thischar[0] < 0x20) || (thischar[0] > 0x7e)) && (thischar[0] != 0xa) && (thischar[0] != 0x9)) {
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
          for (unsigned int charptr = 0; charptr < strlen(templine); charptr++) {
            for (unsigned int goodcharptr = 0; goodcharptr < strlen(goodcodechar); goodcharptr++) {
              if (templine[charptr] == goodcodechar[goodcharptr]) {
                goodchars++;
              }
            }
          }
          // This lines seems that fix the correct generation of the output.
          // Why? I have no idea
          QTextStream(stdout) << QString("Parsed cheat char %1\n").arg(strptr);
          if ((goodchars >= 10) && ((strlen(templine) - goodchars) < 5)) {
            if (goodchars < 20) {
              char tempcpychar[2];
              memset(tempcpychar, 0, 2);
              char* newtempline = (char*)malloc(strlen(templine) + 20);
              memset(newtempline, 0, strlen(templine) + 7);
              int tempcpyptr = 0;
              while ((tempcpychar[0] != ' ') && (tempcpyptr < (int)strlen(templine))) {
                tempcpychar[0] = *(templine + tempcpyptr);
                if (tempcpychar[0] != ' ') {
                  strcat(newtempline, tempcpychar);
                }
                tempcpyptr++;
              }
              if (tempcpyptr == (int)strlen(templine)) {
                tempcpyptr = strlen(templine) - 5;
                *(newtempline + strlen(templine) - 5) = 0;
              }
              strcat(newtempline, " ");
              memset(newtempline + strlen(newtempline), 0, strlen(templine) + 20 - strlen(newtempline));
              memset(newtempline + tempcpyptr + 1, '0', 9 - (strlen(templine) - tempcpyptr));
              sprintf(newtempline + strlen(newtempline), "%s", templine + tempcpyptr);
              labellast = 0;
              sprintf(templine, "%s\n", newtempline);

              if (tempcpyptr == (int)strlen(templine)) {
                QTextStream(stdout) << QString("Bad code detected: %s").arg(templine);
                templine[0] = 0;
              }

              free(newtempline);
            }
            strcat(tempchar, templine);
          } else { // label or bad code
            if (strlen(templine) > 0) {
              templine[strlen(templine) - 1] = 0;
              if ((labellast == 0) && (howmanylines > 1)) {
                strcat(tempchar, "}\n");
              }
              sprintf(tempchar + strlen(tempchar), "[%s]\n{\n", templine);
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

  return tempchar;
}

void
getnextcheatline(char* cheatcodechar, int* chtptr, char* chtline)
{
  memset(chtline, 0, MAXCHTLINE * sizeof(char));
  while (1) {
    if ((cheatcodechar[chtptr[0]] == '{')        /*|| (cheatcodechar[chtptr[0]]=='}')*/
        || (cheatcodechar[chtptr[0]] == '\n')) { // add code here possibly for label with '{'?
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
  char goodcharlist[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F',
                          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ',' };
  for (int chtptr = 0; chtptr < (int)strlen(cheatline); chtptr++) {
    int thismatch = 0;
    for (int goodcharptr = 0; goodcharptr < (int)strlen(goodcharlist); goodcharptr++)
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
  for (int textptr = 0; textptr < (int)strlen(textchar); textptr++)
    if (*(textchar + textptr) == ',')
      commas++;
  return commas;
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

  char* cheatline = (char*)malloc(MAXCHTLINE);
  char* lastdesc = (char*)malloc(MAXCHTLINE);
  char* lastlabel = NULL;
  while (myptr < (int)strlen(cheatcodechar)) {
    getnextchtline(cheatcodechar, &myptr, cheatline);

    if (testcht(cheatline, (char*)"OFF=") == 1)
      continue;
    if (strlen(cheatline) > 0) {
      trim(cheatline, (char*)"[", (char*)"");
      trim(cheatline, (char*)"]", (char*)"");
      trim(cheatline, (char*)",\r\n", (char*)"\r\n");
      trim(cheatline, (char*)";\r\n", (char*)"\r\n");
      trim(cheatline, (char*)"ON=\r\n", (char*)"");
      trim(cheatline, (char*)"ON=", (char*)"");
      trim(cheatline, (char*)"0=", (char*)"");
      trim(cheatline, (char*)"MAX=", (char*)"");
      if (*cheatline == '=')
        trim(cheatline, (char*)"=", (char*)"");
      trim(cheatline, (char*)";", (char*)"\r\n");
      trim(cheatline, (char*)"TEXT=", (char*)"");
      trim(cheatline, (char*)"Text=", (char*)"");
      if (strlen(cheatline) > 1)
        sprintf(tempchar + strlen(tempchar), "%s\r\n", cheatline);
    }
  }

  memset(cheatcodechar, 0, MAXCODELEN);
  memcpy(cheatcodechar, tempchar, strlen(tempchar));
  memset(tempchar, 0, MAXCODELEN);

  myptr = 0;

  while (myptr < (int)strlen(cheatcodechar)) {
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
      for (int copyptr = 0; copyptr < (int)strlen(multimodchar + 1); copyptr++) {
        *(cheatline + copyptr) = *(multimodchar + 1 + copyptr);
      }
      *(cheatline + strlen(multimodchar + 1)) = 0;
    }

    int cheat = testchtline(cheatline);

    if (cheat == 1) {
      trim(cheatline, (char*)":", (char*)",");
      char* addrchar = strstr(cheatline, ",");
      *addrchar = 0;
      addrchar++;
      int addressint = hextoint(cheatline);
      if (addressint == 0x400130) {
        sprintf(tempchar + strlen(tempchar), "D0000020 %04X\r\n", hextoint(addrchar) ^ 0xff);
        continue;
      }
      if ((addressint == 0) || (addressint > 0x41FFFFF)) {
        sprintf(tempchar + strlen(tempchar), "//%08X:%s ;pointer code????\r\n", addressint, addrchar);
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
            sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint, hextoint(addrchar));
            break;
          case 3:
          case 4:
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, hextoint(addrchar));
            break;
          default:
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, hextoint(addrchar) & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, (hextoint(addrchar) >> 16) & 0xffff);
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
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
              break;
            case 2:
              if ((addressint % 2) == 0) { // even address to start
                sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint + 2, values[2]);
              } else {
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint, values[0]);
                sprintf(tempchar + strlen(tempchar), "3%X %02X%02X\r\n", addressint + 1, values[2], values[1]);
              }
              break;
            case 3:
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint, values[1], values[0]);
              sprintf(tempchar + strlen(tempchar), "8%X %02X%02X\r\n", addressint + 2, values[3], values[2]);
              break;
            default:
              int commahalf = ((int)((commacount + 1) / 2));
              sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, commahalf);
              int commaptr = 0;
              while (commaptr < commahalf) {
                int thisval[] = { 0, 0, 0 };
                if (commaptr < commahalf)
                  thisval[0] = ((values[commaptr * 2 + 1]) & 0xff) | ((values[commaptr * 2] & 0xff) << 8);
                if (commaptr + 1 < commahalf)
                  thisval[1] = ((values[commaptr * 2 + 3]) & 0xff) | ((values[commaptr * 2 + 2] & 0xff) << 8);
                if (commaptr + 2 < commahalf)
                  thisval[2] = ((values[commaptr * 2 + 5]) & 0xff) | ((values[commaptr * 2 + 4] & 0xff) << 8);
                sprintf(tempchar + strlen(tempchar), "%04X%04X %04X\r\n", thisval[0], thisval[1], thisval[2]);
                commaptr += 3;
              }
              if ((commacount + 1) % 2 != 0)
                sprintf(tempchar + strlen(tempchar), "3%X %04X\r\n", addressint + commahalf * 2, values[commacount]);
          }
        }
        if (size == 2) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, values[0]);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, values[1]);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, commacount + 1);
            for (int commactr = 0; commactr < commacount + 1; commactr += 3) {
              int thisval[] = { 0, 0, 0 };
              if (commactr < commacount + 1)
                thisval[0] = ((values[commactr] >> 8) & 0xff) | ((values[commactr] & 0xff) << 8);
              if (commactr + 1 < commacount + 1)
                thisval[1] = ((values[commactr + 1] >> 8) & 0xff) | ((values[commactr + 1] & 0xff) << 8);
              if (commactr + 2 < commacount + 1)
                thisval[2] = ((values[commactr + 2] >> 8) & 0xff) | ((values[commactr + 2] & 0xff) << 8);
              sprintf(tempchar + strlen(tempchar), "%04X%04X %04X\r\n", thisval[0], thisval[1], thisval[2]);
            }
          }
        }

        if (size == 4) {
          if (commacount == 1) {
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint, values[0] & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 2, (values[0] >> 16) & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 4, values[1] & 0xffff);
            sprintf(tempchar + strlen(tempchar), "8%X %04X\r\n", addressint + 6, (values[1] >> 16) & 0xffff);
            continue;
          }

          if (commacount > 1) {
            sprintf(tempchar + strlen(tempchar), "5%X %04X\r\n", addressint, (commacount + 1) * 2);
            for (int commactr = 0; commactr < commacount + 1; commactr++) {
              int thisval = ((values[commactr] & 0xff) << 24) | (((values[commactr] >> 8) & 0xff) << 16) |
                            ((values[commactr] & 0xff0000) >> 8) | ((values[commactr] >> 24) & 0xff);
              if ((commactr % 3) == 0)
                sprintf(tempchar + strlen(tempchar), "%08X ", thisval);
              if ((commactr % 3) == 1)
                sprintf(tempchar + strlen(tempchar), "%04X\r\n%04X", (thisval >> 16) & 0xffff, thisval & 0xffff);
              if ((commactr % 3) == 2)
                sprintf(tempchar + strlen(tempchar), "%04X %04X\r\n", (thisval >> 16) & 0xffff, thisval & 0xffff);
            }
            if (((commacount + 1) % 3) == 1)
              sprintf(tempchar + strlen(tempchar), "0000\r\n");
            if (((commacount + 1) % 3) == 2)
              sprintf(tempchar + strlen(tempchar), "0000 0000\r\n");
          }
        }
      }
    } else {
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
