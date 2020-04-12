#define GBAATMICON 3000
#include <stdio.h>
#include <stdlib.h>
//#define DEVING

unsigned int * temptrainermenuint;
//unsigned short * menubgshort;
//unsigned short * menuselectshort;
//unsigned short * menufontshort;
unsigned short * menubgshort=(unsigned short *)malloc(76800);
unsigned short * menuselectshort=(unsigned short *)malloc(6720);
unsigned short * menufontshort=(unsigned short *)malloc(216);
int wantbg=0;
int wantselect=0;
int wantfont=0;
