#include <stdio.h>
#include <stdlib.h>

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
