#include <../functions.h>
#include <../variables.h>

int
convertraw(char* cheatcodes, unsigned int* cheatcodeint, int wantmenu, int cheatselectram, unsigned int* menuint)
{

  unsigned int bytewrite[] = { 0xE3A00000, 0xE59F1004, 0xE5C10000, 0xEA000000, 0xFFFFFFFF };
  unsigned int bigwrite[] = { 0xE59F000C, 0xE59F1004, 0xE1C100B0, 0xEA000001, 0xFFFFFFFF, 0xEEEEEEEE };

  unsigned int loadramreg[] = { 0xE59FB000, 0xEA000000, (unsigned int)cheatselectram };
  unsigned int ramtest[] = { 0xE5DB0000, 0xE59F1008, 0xE0100001, 0xA000002, 0xEA000000 };
  int intcounter = 0;
  int cheatcounter = 0;

#define maxaddr 300

  int menujmp = 0;
  int jumpwidth = 0;
  int whichbit = -1;
  char* tempaddr = (char*)malloc(9 * sizeof(char));

  int* cheatptr = (int*)malloc(2 * sizeof(int));
  char* cheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  char* lastcheatline = (char*)malloc(MAXCHTLINE * sizeof(char));
  cheatptr[0] = 0;
  cheatline[0] = 0;

  if (wantmenu == 1) {
    copyint(cheatcodeint + intcounter, loadramreg, 3);
    intcounter += 3;
  }
  while (cheatptr[0] < (int)strlen(cheatcodes)) {
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

    if ((cheatline[0] == '[') || (cheatline[0] == '{') || (cheatline[0] == '}')) {
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
convertcb(char* cheatcodes, unsigned int* cheatcodeint, int wantmenu, int cheatselectram, unsigned int* menuint)
{

  unsigned int iftopbyte[] = { 0xE3A02000, 0xE59F100C, 0xE1D100B0 };
  unsigned int superint[] = { 0xE59F1014, 0xE28F2014, 0xE0D200B2, 0xE0C100B2, 0xE2533001, 0x1AFFFFFB, 0xEA000000 };
  unsigned int slideint[] = { 0xE1DF21BE, 0xE1DF31BC, 0xE1DF41BA, 0xE08100B2, 0xE0800004, 0xE2533001, 0x1AFFFFFB, 0xEA000002 };
  unsigned int dpadint[] = { 0xE3A01301, 0xE5911130, 0xE3A00C03, 0xE28000FF, 0xE0010000, 0xE1500002, 0x1A000000 };

  unsigned int loadramreg[] = { 0xE59FB000, 0xEA000000, (unsigned int)cheatselectram };
  unsigned int ramtest[] = { 0xE5DB0000, 0xE59F1008, 0xE0100001, 0xA000000, 0xEA000000 };

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
  char* templongchar = (char*)malloc(9 * sizeof(char));

  int encryptionon = 0;
  int slideon = 0;
  int superon = 0;
  int conditionalon = 0;
  int conditionallast = 0;
  int nextconditional = 0;

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

  while (cheatptr[0] < (int)strlen(cheatcodes)) {
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
          *(cheatcodeint + jumppatches[jumpctr]) |= (intcounter - jumppatches[jumpctr] - 2);
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
      if ((((cheatline[0] == '0') && (slideon == 0)) || (cheatline[0] == '1')) && (encryptionon == 0)) {
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
        unsigned char codebuffer[8] = { (unsigned char)(tempaddress & 255),
                                        (unsigned char)((tempaddress >> 8) & 255),
                                        (unsigned char)((tempaddress >> 16) & 255),
                                        (unsigned char)((tempaddress >> 24) & 255),
                                        (unsigned char)(tempvalue & 255),
                                        (unsigned char)((tempvalue >> 8) & 255),
                                        0,
                                        0 };
        cheatsCBADecrypt(codebuffer);
        int* codebuffint = (int*)codebuffer;
        tempaddress = *codebuffint;
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
            *(cheatcodeint + intcounter) = (((decval & 0xff00) >> 8) | ((decval & 0xff) << 8));
            intcounter++;
          }
        } else {
          *(cheatcodeint + intcounter - 1) =
            byteflipint(((((*(cheatcodeint + intcounter - 1) & 0xff) << 8) | ((*(cheatcodeint + intcounter - 1) & 0xff00) >> 8)) << 16) |
                        ((tempdec & 0xffff0000) >> 16)); // byteflipint(>>16)0x7788<<16

          if (superon > 4) {
            *(cheatcodeint + intcounter) = byteflipint(((decval & 0xffff)) | ((tempdec & 0xffff) << 16));
            intcounter++;
          }
        }

        superon -= 6; // 3;

        *(cheatcodeint + superptr) = 0xea000000 | (intcounter - superptr - 2);
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

      if ((cheatline[0] == '2') || (cheatline[0] == '6') || (cheatline[0] == 'E')) {

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

      if ((cheatline[0] == '7') || (cheatline[0] == 'A') || (cheatline[0] == 'B') || (cheatline[0] == 'C') || (cheatline[0] == 'F')) {

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


        // *(menuint+cheatcounter*7+1)=0xEFBEADDE;
        return intcounter;
}
*/
