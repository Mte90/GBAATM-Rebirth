#include <stdio.h>
#include <fstream>
#include <cstring>

void goodname(char * badname) {
    char * tempchar=(char *)malloc(300*sizeof(char)); //strlen(badname)+20);
    memset(tempchar,0,300*sizeof(char));
    int charpointer=0;
    for (unsigned int i=0; i<strlen(badname); i++) {
        if ((*(badname+i)!=0x22)) {
            *(tempchar+charpointer)=*(badname+i);
            charpointer++;
        }
    }
    strcpy(badname,tempchar);
}

int bmp2short(char * bmpfilestr, unsigned short * shortbuffer, int whichpic) {

char tempchar[300];

goodname(bmpfilestr);
FILE * bmpfile=fopen(bmpfilestr,"rb");
fseek(bmpfile,0,SEEK_END);
int bmplen=ftell(bmpfile);
rewind(bmpfile);
unsigned char * bmpfilechar=(unsigned char *)malloc(bmplen);
int bytesread=fread(bmpfilechar,1,bmplen,bmpfile);
fclose(bmpfile);
if (bytesread!=bmplen) {
	free(bmpfilechar);
	sprintf(tempchar,"Error reading bitmap");

	return -1;
}
if ((*bmpfilechar!='B') || (*(bmpfilechar+1)!='M')) {
	free(bmpfilechar);
	sprintf(tempchar,"Nice try... But I need a real bitmap...");

	return -1;
}

unsigned int * bmpint=(unsigned int *)(bmpfilechar+2);
//int filelen=(*(bmpfilechar+5)<<24)|(*(bmpfilechar+4)<<16)|(*(bmpfilechar+3)<<8)|*(bmpfilechar+2);
int bmppixstart=*(bmpint+2);
int bmpwidth=*(bmpint+4);
int bmpheight=*(bmpint+5);
int bmppixlen=*(bmpint+8);
int bmpbpp=(*(bmpint+6)>>16);
int bmpbytesperline=((bmpwidth*bmpbpp-1)/32)*4+4;
if (*bmpint!=bmplen) {
	free(bmpfilechar);
	sprintf(tempchar,"The bitmap length doesn't match! It is corrupted!");

	return -1;
}
int gooddims=1;
if ((whichpic==1) && ((bmpwidth!=240)||(bmpheight!=160))) {
	gooddims=0;
	sprintf(tempchar,"The background bitmap needs to be 240x160");
}
if ((whichpic==2) && ((bmpwidth!=240)||(bmpheight!=14))) {
	gooddims=0;
	sprintf(tempchar,"The selection bar bitmap needs to be 240x14");
}
if ((whichpic==3) && ((bmpwidth!=9)||(bmpheight!=12))) {
	gooddims=0;
	sprintf(tempchar,"The font texture bitmap needs to be 9x12");
}
if (gooddims==0) {
    free(bmpfilechar);
	return -1;
}

unsigned short thisshort;
unsigned char r;
unsigned char g;
unsigned char b;
unsigned char * pixchar=(bmpfilechar+bmppixstart);
//bmpbpp=3;
int thisline=0;
for (int y=0; y<bmpheight; y++) {
	for (int x=0; x<bmpwidth; x++) {
		b=*(pixchar+thisline+x*bmpbpp/8+2);
		g=*(pixchar+thisline+x*bmpbpp/8+1);
		r=*(pixchar+thisline+x*bmpbpp/8);
		thisshort=((r>>3)<<10)|((g>>3)<<5)|(b>>3);
		*(shortbuffer+(bmpheight-1-y)*bmpwidth+x)=thisshort;
	}
	thisline+=bmpbytesperline;
}

#ifdef DEVING
	sprintf(tempchar,"BMP info - width: %d height: %d bpp: %d bytes per line: %d",bmpwidth,bmpheight,bmpbpp, bmpbytesperline);
// 	AddList(hSTATUSLIST,tempchar);
// 	ScrollEnd(hSTATUSLIST);
sprintf(bmpfilestr+strlen(bmpfilestr)-4,".bin");
FILE * bmpoutfile=fopen(bmpfilestr,"wb");
fwrite(shortbuffer,1,bmpwidth*bmpheight*2,bmpoutfile);
fclose(bmpoutfile);
#endif

	sprintf(tempchar,"Bitmap imported successfully");

free(bmpfilechar);
return 1;
}
