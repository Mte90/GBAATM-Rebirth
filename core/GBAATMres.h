#include <stdio.h>
#include <stdlib.h>

//unsigned short * menubgshort=(unsigned short *)malloc(76800);
//unsigned short * menuselectshort=(unsigned short *)malloc(6720);
//unsigned short * menufontshort=(unsigned short *)malloc(216);
//int wantbg=0;
//int wantselect=0;
//int wantfont=0;

struct INIVALUES {
    unsigned int * armtoembed;
        unsigned int * armtohook;
    unsigned int * traineraddr;
    char * gamenamestr;
        char  * gameidstr;
};

struct ADDRESSSTRUCT {
    unsigned int * oldasmaddrs;
    unsigned int * asmaddr;
};

struct LVALSTRUCT {
    unsigned int * oldasmlvalues;
    unsigned int * asmlvalue;
};

struct ENABLEDISABLESTRUCT {
    int wantenable;
    int enablekey;
    int disablekey;
    char enablekeystr[50];
    char disablekeystr[50];
};

struct SLOMOSTRUCT {
    int wantslomo;
    int slowdownkey;
    int speedupkey;
    char slowdownkeystr[50];
    char speedupkeystr[50];
};

ENABLEDISABLESTRUCT myedstruct;
SLOMOSTRUCT myslomostruct;
