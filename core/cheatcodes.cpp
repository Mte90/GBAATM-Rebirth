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
