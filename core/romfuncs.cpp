#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include "GBAATMres.h"
#include "trainermenu.h"
#include <QString>
#define SIZEOFHOOKJUMP 10 //7
int ConvertKeys(char * keystr) {
	char * keys[] = {"L","R","DOWN","UP","LEFT","RIGHT","START","SELECT","B","A"};
int keycode=0x3ff;
	for (int toupperptr=0; toupperptr<strlen(keystr); toupperptr++) {
		keystr[toupperptr]=toupper(keystr[toupperptr]);
	}
	
char tempstr[100];
tempstr[0]=0;
for (int thiskey=0; thiskey<10; thiskey++) {
	if (strstr(keystr,keys[thiskey])) {
		if ((thiskey==0) || (thiskey==1) || (thiskey==9)) {

			if (thiskey==0) {
				if (*(strstr(keystr,keys[0])+1)!='E') {
					keycode^=1<<(9-thiskey);
					sprintf(tempstr+strlen(tempstr),"+%s",keys[thiskey]);
				}
			}

			if (thiskey==1) {
				if ((*(strstr(keystr,keys[1])+1)!='I') && (*(strstr(keystr,keys[1])+1)!='T')) {
					keycode^=1<<(9-thiskey);
					sprintf(tempstr+strlen(tempstr),"+%s",keys[thiskey]);
				}
			}

			if (thiskey==9) {
				if (*(strstr(keystr,keys[9])-2)!='S') {
					keycode^=1<<(9-thiskey);
					sprintf(tempstr+strlen(tempstr),"+%s",keys[thiskey]);
				}
			}

		}
		else {
			if ( ((thiskey==3) && (!strstr(keystr,keys[2]))) ||
				((thiskey==5) && (!strstr(keystr,keys[4]))) ||
				((thiskey<3) || (thiskey==4) || (thiskey>5)) ) {
			keycode^=1<<(9-thiskey);
			sprintf(tempstr+strlen(tempstr),"+%s",keys[thiskey]);
			}
		}
	}

}
if (tempstr[0]=='+') {
	sprintf(keystr,"%s",tempstr+1);
	}
	else {
	sprintf(keystr,"%s",tempstr);
	}

return keycode;
}

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

int fileexists(const char * filename) {
	FILE * thisfile=fopen(filename,"rb");
	if (thisfile) {
		fclose(thisfile);
		return 1;
	}
	else {
		return 0;
	}
}


int hextoint(char *hexstr) {
int pos=0;
char hexmap[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
int returnval=-1;   
while (hexstr[pos]!=0) {
//int returntemp=returnval;
for(int i=0; i<22; i++) {
if (hexstr[pos]==hexmap[i]) {
	if (returnval==-1) { returnval=0; }
returnval<<=4;
if (i<16) {
returnval|=i;         
}
else {
returnval|=(i-6);    
}                             
}        
}
//if (returnval==returntemp) {
//return -1;                           
//}
pos++;
}   
return returnval;        
}

void flippath(char * path) {
	for(int strptr=0; strptr<strlen(path); strptr++) {
		if (path[strptr]=='\\') {
			path[strptr]=(char)'/';
		}
		else if (path[strptr]=='/') {
			path[strptr]=(char)'\\';
		}
	}
}
			
void formatfopenstr(char * path) {
	for(int strptr=0; strptr<strlen(path); strptr++) {
		if (path[strptr]=='\\') {
			path[strptr]=(char)'/';
		}
	}
}

void formatsystemstr(char * path) {
	for(int strptr=0; strptr<strlen(path); strptr++) {
		if (path[strptr]=='\\') {
			path[strptr]=(char)'/';
		}
	}
}


void getpathfromfilename(char * filename, char * path) {
	int lastslash;
lastslash=-1;
for (int charptr=0; charptr<strlen(filename); charptr++) {
if ((filename[charptr]=='\\') || (filename[charptr]=='/')) {
lastslash=charptr;
}
}
memcpy(path,filename,lastslash+1);
path[lastslash+1]=0;
}

void copyint(unsigned int * destint,unsigned int * srcint,int numint) {
	for (int intptr=0; intptr<numint; intptr++) {
		*(destint+intptr)=*(srcint+intptr);
	}
}
void strright(char * srcstr, char * deststr, unsigned int chartocpy) {
	for (unsigned int charcpy=strlen(srcstr)-chartocpy; charcpy<strlen(srcstr); charcpy++) {
		deststr[charcpy-(strlen(srcstr)-chartocpy)]=srcstr[charcpy];
	}
	deststr[chartocpy]=0;
}

void strleft(char * srcstr, char * deststr, unsigned int chartocpy) {
	for (unsigned int charcpy=0; charcpy<chartocpy; charcpy++) {
		deststr[charcpy]=srcstr[charcpy];
	}
	deststr[chartocpy]=0;
}

void strmid(char * srcstr, char * deststr, unsigned int charstart,unsigned int chartocpy) {
	for (unsigned int charcpy=0; charcpy<chartocpy; charcpy++) {
		deststr[charcpy]=srcstr[charcpy+charstart-1];
	}
	deststr[chartocpy]=0;
}

void stripnpc(char * stringtostrip) {
	char * tempstr=(char *)malloc(strlen(stringtostrip)+100);
	memset(tempstr,0,strlen(stringtostrip)+100);
	int tempptr=0;
	for (int charptr=0; charptr<strlen(stringtostrip); charptr++) {
	char thischar=*(stringtostrip+charptr);

	if ((thischar>=0x20) && (thischar<=0x7a)) {
		//if ((thischar!=0x5b) && (thischar!=0x5d)) {
			*(tempstr+tempptr)=thischar;
			tempptr++;
		//}
	}

	if (thischar==0xa) {
		*(tempstr+tempptr)='\r';
		*(tempstr+tempptr+1)='\n';
		tempptr+=2;
	}
/*	else {
				*(tempstr+tempptr)=thischar;
		tempptr++;
	}
*/
	}
	memset(stringtostrip,0,strlen(stringtostrip)+1);
	memcpy(stringtostrip,tempstr,strlen(tempstr)+1);
	free(tempstr);
}

void getromname(char * gbaromstr,char * tempname) {
	char * gbachar=(char *)malloc(0xb0);
	formatfopenstr(gbaromstr);
	FILE * gbaromfile=fopen(gbaromstr,"rb");
	if (gbaromfile) {
		fread(gbachar,1,0xb0,gbaromfile);
		fclose(gbaromfile);
	memset(tempname,0,25);
	memcpy(tempname,gbachar+0xa0,12);
	sprintf(tempname+strlen(tempname)," - [");
    memcpy(tempname+strlen(tempname),gbachar+0xac,4);
	sprintf(tempname+strlen(tempname),"]");
	free(gbachar);
	}
}

int findromend(unsigned int * gbaint,int gbaeof) {
//int padint=gbaint[(gbaeof/4)-1];
//if ((padint!=0xffffff) && (padint!=0x0)) return gbaeof;
int lastnonpad=-1;
	for (int gbaptr=0; gbaptr<(gbaeof/4)-1; gbaptr++) {
		//if (gbaint[gbaptr]!=padint) { lastnonpad=gbaptr; }
		if (((gbaint[gbaptr]!=0xffffffff) && (gbaint[gbaptr]!=0x0)) || (gbaint[gbaptr]!=gbaint[gbaptr+1])) { lastnonpad=gbaptr; }
	}
	return (lastnonpad)*4;
}
/*
void findhooks(int * gbaint,int gbaeof) {
for(int gbaptr=0; gbaptr<(gbaeof-gbaIDlen)/4; gbaptr++) {

if (((gbaint[gbaptr]&0xffff0fff)==0xe3a00301) && ((gbaint[gbaptr+1]&0xfff00fff)==0xe2800c02) && ((gbaint[gbaptr+2]&0xfff00fff)==0xe5900000)) {

	
	printf("Irq address found at 0x%X using r%d\n\n",0x8000000+gbaptr*4,((gbaint[gbaptr]&0xf000)>>12));
}

}
}
*/

QString deadbeefrom(char * gbaromname,char * newgbaromname) {
	//unsigned int gbadeadbeefint[]={0xE59F002C,0xE3A01402,0xE3A02801,0xE4810004,0xE2522001,0x1AFFFFFC,0xE3A01403,0xE3A02A02,0xE4810004,0xE2522001,0x1AFFFFFC,0xE51FF004,0x80000C0,0xEFBEADDE};
	unsigned int gbadeadbeefint[]={0xE59F0018,0xE3A01403,0xE3A02A02,0xE4810004,0xE2522001,0x1AFFFFFC,0xE51FF004,0x0,0xEFBEADDE};
	
    #define SIZEOFDBFUNC 9
	
    char tempchar[300];
    QString code = "-1";
	unsigned int * gbaromint=NULL;
	formatfopenstr(gbaromname);
    FILE * gbafile=fopen(gbaromname,"rb");
    if (gbafile) {
            fseek(gbafile,0,SEEK_END);
            int gbalen=ftell(gbafile);
            rewind(gbafile);
            gbaromint=(unsigned int *)malloc(gbalen+SIZEOFDBFUNC*4);
            fread(gbaromint,1,gbalen,gbafile);
            unsigned int fillint=*(gbaromint+(gbalen/4)-1);
            if ((fillint!=0xffffffff) && (fillint!=0x0)) { fillint=0x0; }
                for (int fillctr=0; fillctr<0xe; fillctr++) {
                    *(gbaromint+(gbalen/4)+fillctr)=fillint;
            }
            fclose(gbafile);
            int realgbaend=findromend(gbaromint,gbalen+SIZEOFDBFUNC*4);
            sprintf(tempchar,"Free space found at 0x%X",realgbaend+0x8000004);

            copyint(gbaromint+(realgbaend+4)/4,gbadeadbeefint,SIZEOFDBFUNC);
            *(gbaromint+(realgbaend+4)/4+SIZEOFDBFUNC-2)=0x8000000|((*gbaromint&0xffffff)*4+8);
            *gbaromint=0xea000000|((realgbaend/4)-1);
            if (fileexists(newgbaromname)==1) { remove(newgbaromname); }
            formatfopenstr(newgbaromname);
            FILE * newgbaromfile=fopen(newgbaromname,"wb");
            if (newgbaromfile) {
                realgbaend=findromend(gbaromint,gbalen+SIZEOFDBFUNC*4);
                int byteswritten=fwrite(gbaromint,1,realgbaend+4,newgbaromfile);
                fclose(newgbaromfile);
                if (byteswritten!=realgbaend+4) {
                        free(gbaromint);
                        return code;
                }
            } else {
                    free(gbaromint);
                    return code;
            }
	}	else {
        free(gbaromint);
        return code;
	}
	
	free(gbaromint);
    return tempchar;
}

int patchrom(char * gbaromname,char * newgbaromname,unsigned int * mycheatint, int cheatintlen,int freeram, SLOMOSTRUCT slomostruct,ENABLEDISABLESTRUCT edstruct,int excycles,char * path,int wantmenu,unsigned int * menuint, int cheatselectram, int vblankcheck) {


	//const char edstrdis[]="stmdb r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\nldr r1,settingsram\nldrb r4,[r1]\nldr r2,enablekey\nldr r3,disablekey\nmov r0,#0x4000000\nldr r0,[r0,#+0x130]\ncmp r0,r2\nmoveq r4,#0x1\ncmp r0,r3\nmoveq r4,#0x0\nstrb r4,[r1]\ncmp r4,#0x0\nbne turnedon\nldmia r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\nbx r14\nturnedon:\nldmia r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\n\n";
	//const char edstren[]="stmdb r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\nldr r1,settingsram\nldrb r4,[r1]\nldr r2,enablekey\nldr r3,disablekey\nmov r0,#0x4000000\nldr r0,[r0,#+0x130]\ncmp r0,r2\nmoveq r4,#0x0\ncmp r0,r3\nmoveq r4,#0x1\nstrb r4,[r1]\ncmp r4,#0x0\nbeq turnedon\nldmia r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\nbx r14\nturnedon:\nldmia r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\n\n";
	//const char smstr[]="stmdb r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\nmov r2,#0x0\nldr r4,slowdownkey\nldr r5,speedupkey\nmov r1,#0x4000000\nldr r1,[r1,#+0x130]\nldr r6,settingsram\nldrb r0,[r6,#+0x2]\ncmp r1,r4\nmoveq r2,#0x1\naddeq r0,#0x8\ncmp r0,#0xc0\nmovgt r0,#0xc0\ncmp r1,r5\nmoveq r2,#0x1\nsubeqs r0,#0x10\nmovlt r0,#0x0\nldrb r1,[r6,#+0x1]\ncmp r1,#0x0\nstreqb r0,[r6,#+0x2]\nstrb r2,[r6,#+0x1]\nslowmoloop:\nmov r1,#0x40\nslowmoinnerloop:\nsubs r1,r1,#0x1\nbne slowmoinnerloop\nsubs r0,r0,#0x1\nbcs slowmoloop\nslowmodone:\nldmia r13!,{r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14}\n\n";
	//const char edstrdis[]="ldr r1,settingsram\nldrb r4,[r1]\nldr r2,enablekey\nldr r3,disablekey\nmov r0,#0x4000000\nldr r0,[r0,#+0x130]\ncmp r0,r2\nmoveq r4,#0x1\ncmp r0,r3\nmoveq r4,#0x0\nstrb r4,[r1]\ncmp r4,#0x0\nbne turnedon\nbx r14\nturnedon:\n\n";
	//const char edstren[]="ldr r1,settingsram\nldrb r4,[r1]\nldr r2,enablekey\nldr r3,disablekey\nmov r0,#0x4000000\nldr r0,[r0,#+0x130]\ncmp r0,r2\nmoveq r4,#0x0\ncmp r0,r3\nmoveq r4,#0x1\nstrb r4,[r1]\ncmp r4,#0x1\nbne turnedon\nbx r14\nturnedon:\n\n";
	//const char smstr[]="mov r2,#0x0\nldr r4,slowdownkey\nldr r5,speedupkey\nmov r1,#0x4000000\nldr r1,[r1,#+0x130]\nldr r6,settingsram\nldrb r0,[r6,#+0x2]\ncmp r1,r4\nmoveq r2,#0x1\naddeq r0,#0x2\ncmp r0,#0xfe\nmovgt r0,#0xfe\ncmp r1,r5\nmoveq r2,#0x1\nsubeqs r0,#0x2\nmovlt r0,#0x0\nldrb r1,[r6,#+0x1]\ncmp r1,#0x0\nstreqb r0,[r6,#+0x2]\ncmp r0,#0x0\nbeq slomodone\nstrb r2,[r6,#+0x1]\nslowmoloop:\nmov r1,#0x38\nslowmoinnerloop:\nsubs r1,r1,#0x1\nbne slowmoinnerloop\nsubs r0,r0,#0x1\nbcs slowmoloop\nslowmodone:\n\n";
	//const char execstr[]="ldr r1,settingsram\nldrb r2,[r1,#+0x3]\nldr r3,executioncycles\nadd r2,#0x1\ncmp r2,r3\nmoveq r2,#0x0\nstrb r2,[r1,#+0x3]\nbxne r14\n\n";
	//const char vblankstr[]="ldr r1,[r15,#+0xc]\nldr r0,[r1]\nands r0,r0,#0x1\nbne cheatstart\nbx r14\n.long 0x4000202\ncheatstart:\n\n";


	unsigned int eddisint[]={0xE59F103C,0xE5D14000,0xE1DF23BA,0xE1DF33B4,0xE3A00301,0xE5900130,0xE3A05C03,0xE28550FF,0xE0000005,0xE1500002,0x3A04001,0xE1500003,0x3A04000,0xE5C14000,0xE3540000,0x1A000002,0xE12FFF1E,0xDDDDDDDD,0xEEEEFFFF};

	unsigned int edenint[]={0xE59F103C,0xE5D14000,0xE1DF23BA,0xE1DF33B4,0xE3A00301,0xE5900130,0xE3A05C03,0xE28550FF,0xE0000005,0xE1500002,0x3A04000,0xE1500003,0x3A04001,0xE5C14000,0xE3540001,0x1A000002,0xE12FFF1E,0xDDDDDDDD,0xEEEEFFFF};

	unsigned int slomoint[]={0xE3A02000,0xE1DF47BE,0xE1DF57B8,0xE3A01301,0xE5911130,0xE3A03C03,0xE28330FF,0xE0011003,0xE59F605C,0xE5D60002,0xE1510004,0x3A02001,0x2800002,0xE35000FE,0xC3A000FE,0xE1510005,0x3A02001,0x2500002,0xB3A00000,0xE5D61001,0xE3510000,0x5C60002,0xE3500000,0xA00000A,0xE5C62001,0xE3A01088,0xE1A00000,0xE1A00000,0xE2511001,0x1AFFFFFB,0xE2500001,0x3A000002,0x2AFFFFF7,0xDDDDDDDD,0xEEEEFFFF};
	//unsigned int slomoint[]={0xE3A02000,0xE1DF47B6,0xE1DF57B0,0xE3A01301,0xE5911130,0xE3A03C03,0xE28330FF,0xE0011003,0xE59F6054,0xE5D60002,0xE1510004,0x3A02001,0x2800002,0xE35000FE,0xC3A000FE,0xE1510005,0x3A02001,0x2500002,0xB3A00000,0xE5D61001,0xE3510000,0x5C60002,0xE3500000,0xA000008,0xE5C62001,0xE3A01038,0xE2511001,0x1AFFFFFD,0xE2500001,0x3A000002,0x2AFFFFF9,0xDDDDDDDD,0xEEEEFFFF};
	//unsigned int eddisint[]={0xE59F1030,0xE5D14000,0xE1DF22BE,0xE1DF32B8,0xE3A00301,0xE5900130,0xE1500002,0x3A04001,0xE1500003,0x3A04000,0xE5C14000,0xE3540000,0x1A000002,0xE12FFF1E,0xDDDDDDDD,0xEEEEFFFF};
	//unsigned int edenint[]={0xE59F1030,0xE5D14000,0xE1DF22BE,0xE1DF32B8,0xE3A00301,0xE5900130,0xE1500002,0x3A04000,0xE1500003,0x3A04001,0xE5C14000,0xE3540001,0x1A000002,0xE12FFF1E,0xDDDDDDDD,0xEEEEFFFF};
	//unsigned int slomoint[]={0xE3A02000,0xE1DF46BA,0xE1DF56B4,0xE3A01301,0xE5911130,0xE59F6054,0xE5D60002,0xE1510004,0x3A02001,0x2800002,0xE35000FE,0xC3A000FE,0xE1510005,0x3A02001,0x2500002,0xB3A00000,0xE5D61001,0xE3510000,0x5C60002,0xE3500000,0xA000008,0xE5C62001,0xE3A01038,0xE2511001,0x1AFFFFFD,0xE2500001,0x3A000002,0x2AFFFFF9,0xDDDDDDDD,0xEEEEFFFF};
	unsigned int execint[]={0xE59F101C,0xE5D12003,0xE3A03000,0xE2822001,0xE1520003,0x3A02000,0xE5C12003,0xA000001,0xE12FFF1E,0xFFFFFFFF};
	//unsigned int vblankint[]={0xE59F100C,0xE5910000,0xE2100001,0x1A000001,0xE12FFF1E,0x4000204};
	unsigned int vblankint[]={0xE59F100C,0xE5910000,0xE35000A0,0xAA000001,0xE12FFF1E,0x4000206};
	unsigned int trainerigmint[]={0xE3A01301,0xE591B130,0xE59F2008,0xE15B0002,0xA000000,0xEA000000,0x35b}; //select+down+left

	//unsigned int hookint[]={0xE92D40FF,0xEB000000,0xE8BD40FF,0xE3A03301,0xE2833C02,0xE5932000,0xE52D0008,0xE59F0004,0xE58D0004,0xE8BD8001,0xffffffff};

	#define TRAINERINTMAX 0x4000

	int menupatch=0;
	int menustart=0;

	char tempchar[300];
	//int freeram=0x3007Fe0;
	//ErrorMessage(cheatcodechar);
	int objstart,objend;

	unsigned int * trainerint = (unsigned int *)malloc(TRAINERINTMAX);
	int trainerintptr = 0;
	memset(trainerint, 0, TRAINERINTMAX);
	formatfopenstr(gbaromname);
	FILE * gbafile = fopen(gbaromname, "rb");
	if (gbafile) {
		fseek(gbafile,0,SEEK_END);
		int gbalen = ftell(gbafile);
		rewind(gbafile);
		unsigned int * gbaromint=(unsigned int *)malloc(gbalen + 0x20000);
		fread(gbaromint, 1, gbalen, gbafile);
		unsigned int fillint = *(gbaromint + (gbalen / 4) - 1);
		if ((fillint != 0xffffffff) && (fillint != 0x0)) fillint = 0x0;
		for (int fillctr = 0; fillctr < 0x8000; fillctr++) {
			*(gbaromint + (gbalen / 4) + fillctr) = fillint;
		}
		fclose(gbafile);
		int realgbaend = findromend(gbaromint,gbalen + 0x20000);
		sprintf(tempchar, "Free space found at 0x%X", realgbaend + 0x8000004);
// 		AddList(hSTATUSLIST, tempchar);
// 		ScrollEnd(hSTATUSLIST);


		int spaceneeded = 4;
		if (cheatintlen > 0) spaceneeded = cheatintlen;
		spaceneeded += 924;
		if (wantmenu == 1) spaceneeded += trainermenuint[0] + 28 + 340;
		if (excycles > 1) spaceneeded += 40;
		if ((edstruct.wantenable == 3) || (edstruct.wantenable == 1)) spaceneeded += 76;
		if (slomostruct.wantslomo == 1) spaceneeded += 132;
		if (vblankcheck == 0) spaceneeded -= 6;


		int gbahookaddr[10];
		int gbahookreg[10];
		int gbahooktype[10];
		int gbahooks=-1;

		int temphookaddr=0;
		int temphookreg=0;

		char regtaken[16];
		memset(regtaken,0,16);

		for(int gbaptr=0; gbaptr<(realgbaend-12)/4; gbaptr++) {


			//new hook detect
			if (((gbaromint[gbaptr]&0xffff0fff)==0xe3a00301 /*mov r[5th],*/) && ((gbaromint[gbaptr+1]&0xfff00fff)==0xe2800c02 /*add 4th,5th - badreg*/) && ((gbaromint[gbaptr+2]&0xfff00fff)==0xe5d00008 /*ldr 4th,5th - badreg*/) && ((gbaromint[gbaptr+2]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ )) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=gbaptr*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1;
			}
			//end new hook detect




			if (((gbaromint[gbaptr]&0xffff0fff)==0xe3a00301 /*mov r[5th],*/) && ((gbaromint[gbaptr+1]&0xfff00fff)==0xe2800c02 /*add 4th,5th - badreg*/) && ((gbaromint[gbaptr+2]&0xfff00fff)==0xe5900000 /*ldr 4th,5th - badreg*/) && ((gbaromint[gbaptr+2]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ )) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=gbaptr*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1;
			}

			if (((gbaromint[gbaptr]&0xffff0000)==0xe92d0000 /*push*/) && ((gbaromint[gbaptr+1]&0xffff0fff)==0xe3a00301 /*mov r[5th],*/) && ((gbaromint[gbaptr+2]&0xfff00fff)==0xe5b00200 /*ldr - 4th,5th = bad reg*/ ) && ((gbaromint[gbaptr+3]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ )) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=(gbaptr+1)*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1; //2;
			}

			if (((gbaromint[gbaptr]&0xffff0fff)==0xe3a00640 /*mov 5th],*/) && ((gbaromint[gbaptr+1]&0xfff00fff)==0xe5b00200 /*ldr - 4th,5th = bad reg*/) && ((gbaromint[gbaptr+2]&0xfff00000)==0xe1d00000 /*ldr? - 4th,5th = bad reg*/ ) && ((gbaromint[gbaptr+5]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ ) && ((gbaromint[gbaptr+6]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ ) && ((gbaromint[gbaptr+7]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ )) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=(gbaptr+5)*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1; //3;
			}

			if (((gbaromint[gbaptr]&0xffff0fff)==0xe3a00301) && ((gbaromint[gbaptr+1]&0xfff00fff)==0xe5b00200) && ((gbaromint[gbaptr+2]&0xfff00fff)==0xe1d000b8)) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=gbaptr*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1; //4;
			}


			if (((gbaromint[gbaptr]&0xffff0000)==0xe59f0000 /*ldr rX,[r15,+]*/) && ((gbaromint[gbaptr+1]&0xfff000ff)==0xe5900000 /*ldr rX*/)  && ((gbaromint[gbaptr+1]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ ) && ((gbaromint[gbaptr+2]&0xffff0000)==0xe1a00000 /*mov*/ ) && ((gbaromint[gbaptr+3]&0xffff0000)!=0xe59f0000 /*ldr rX,[r15,+]*/ )) { // && ((gbaromint[gbaptr+4]&0xffff0000)!=0xe3a00000)) {
				temphookaddr=(gbaptr+1)*4;
				temphookreg=(gbaromint[gbaptr]&0xf000)>>12;
				gbahooktype[gbahooks+1]=1; //long jump
			}


									
			if (temphookaddr!=0) { //((gbaromint[gbaptr]==0x3007FFC) && (temphookaddr!=0)) {
				gbahooks++;
				gbahookaddr[gbahooks]=temphookaddr;
				gbahookreg[gbahooks]=temphookreg;
				sprintf(tempchar,"IRQ found at 0x%X",temphookaddr+0x8000000);
// 				AddList(hSTATUSLIST,tempchar);
// 				ScrollEnd(hSTATUSLIST);
				temphookaddr=0;
				temphookreg=0;
				if (gbahooks==9) break;
			}

		}

		if (gbahooks==-1) {
			sprintf(tempchar,"No IRQs found!");
// 			AddList(hSTATUSLIST,tempchar);
// 			ScrollEnd(hSTATUSLIST);
			free(gbaromint);
			return -1;
		}

		spaceneeded+=(gbahooks+1)*SIZEOFHOOKJUMP*4;
		spaceneeded=((int)((spaceneeded+3)/4))*4;

		if ((realgbaend+spaceneeded)>0x1000000) {
			/*AddList(hSTATUSLIST,"The game will be larger than 16MB. Many flash carts have a 16MB limit if the games run from PSRAM so it may not work!");
			ScrollEnd(hSTATUSLIST);		*/	
		}

		if ((realgbaend+spaceneeded)>0x2000000) {
// 			MessageBox(NULL,"The max size a GBA game can be is 32MB. There is not enough space at the end of this game.\r\nThe game will be trimmed to the proper size but it may result graphics corruption, etc.","Oh crap",MB_OK);
			realgbaend=0x2000000-spaceneeded;
			sprintf(tempchar,"The game was trimmed to 0x8%07X",realgbaend*4);
// 			AddList(hSTATUSLIST,tempchar);
// 			ScrollEnd(hSTATUSLIST);
		}

		int oktopatch = 1;

		for (int hookctr = 0; hookctr < gbahooks + 1; hookctr++) {

				for (int opctr = 0; opctr < 4; opctr++) {
					int thisop = gbaromint[gbahookaddr[hookctr] / 4 + opctr];

					if (((thisop & 0xf0000) == 0xd0000) || ((thisop & 0xf000) == 0xd000)) oktopatch = -1;

					//int oc = ((thisop >> 24) & 0xf);
				}

		if (oktopatch == -1) {
			sprintf(tempchar,"Patch not applied to %08", gbahookaddr[hookctr] + 0x8000000);
// 			AddList(hSTATUSLIST,tempchar);
// 			ScrollEnd(hSTATUSLIST);
			continue;
		}

				*(trainerint+trainerintptr)=0xE92D48FF; //push r0-r7,r11,r14
				*(trainerint+trainerintptr+1)=0xEB000000|(((gbahooks-hookctr)*SIZEOFHOOKJUMP+(SIZEOFHOOKJUMP-3))); //bl trainerfunc
				
				*(trainerint + trainerintptr + 2) = 0xE51D005A; //ldr r0, [r13, -5A]
				*(trainerint + trainerintptr + 3) = 0xE2800008; //add r0, 8
				*(trainerint + trainerintptr + 4) = 0xE50D005A; //str r0, [r13, -5A]

				*(trainerint+trainerintptr+5)=0xE8BD48FF; //pop r0-r7,r11,r14
				*(trainerint+trainerintptr+6)=gbaromint[gbahookaddr[hookctr]/4];
				*(trainerint+trainerintptr+7)=gbaromint[gbahookaddr[hookctr]/4+1];
				*(trainerint+trainerintptr+8)=gbaromint[gbahookaddr[hookctr]/4+2];

//				*(trainerint + trainerintptr + 6) = 0xE12FFF10 | regtouse; //0xE12FFF1E;

				*(trainerint + trainerintptr + 9) = 0xE8BD8000; //pop r15

				//*(trainerint+trainerintptr+6)=gbaromint[gbahookaddr[hookctr]/4+3];
				//*(trainerint+trainerintptr+7)=gbaromint[gbahookaddr[hookctr]/4+4];
				//*(trainerint+trainerintptr+8)=gbaromint[gbahookaddr[hookctr]/4+5];

			//}

			
			//if (gbahooktype[hookctr]==2) {

				//*(trainerint+trainerintptr)=0xE92D40FF; //push r0-r12,r14
				//gbaromint[gbahookaddr[hookctr]/4+1]=0xE51FF004;
				//*(trainerint+trainerintptr+1)=0xEB000000|(((gbahooks-hookctr)*SIZEOFHOOKJUMP)+8); //bl trainerfunc

				//*(trainerint+trainerintptr+2)=0xE8BD40FF; //pop r0-r12,r14
				//*(trainerint+trainerintptr+3)=0xE8BD0004; //pop r2
				//*(trainerint+trainerintptr+4)=gbaromint[gbahookaddr[hookctr]/4];
				//*(trainerint+trainerintptr+5)=gbaromint[gbahookaddr[hookctr]/4+1];
				//*(trainerint+trainerintptr+6)=gbaromint[gbahookaddr[hookctr]/4+2];
				//*(trainerint+trainerintptr+7)=gbaromint[gbahookaddr[hookctr]/4+3];
				//*(trainerint+trainerintptr+8)=gbaromint[gbahookaddr[hookctr]/4+4];
				//*(trainerint+trainerintptr+9)=gbaromint[gbahookaddr[hookctr]/4+5];
				//*(trainerint+trainerintptr+9)=0xE92D0004; //push r2
				//*(trainerint+trainerintptr+10)=0xE12FFF1E;

			//gbaromint[gbahookaddr[hookctr]/4]=0xE28F0004|(regtouse<<12); //add r14,=return
			//gbaromint[gbahookaddr[hookctr]/4+1]=0xE51FF004; //ldr r15,traineraddr
			//gbaromint[gbahookaddr[hookctr]/4+2]=realgbaend+0x8000004+hookctr*SIZEOFHOOKJUMP; //traineraddr
			//trainerintptr+=2; //SIZEOFHOOKJUMP;

			//}
			//else {
			
			//gbaromint[gbahookaddr[hookctr]/4]=0xE92D4000; //push r14
			//gbaromint[gbahookaddr[hookctr]/4+1]=0xE28FE004; //0xE28FE008; //add r14,=returnaddr
			//gbaromint[gbahookaddr[hookctr]/4+2]=0xE51FF004; //ldr r15,traineraddr
			//gbaromint[gbahookaddr[hookctr]/4+3]=realgbaend+0x8000004+hookctr*SIZEOFHOOKJUMP; //traineraddr
			//gbaromint[gbahookaddr[hookctr]/4+4]=0xE8BD4000; // pop r14
			
/* newly commented out
			gbaromint[gbahookaddr[hookctr]/4]=0xE28F0004|(regtouse<<12); //add r14,=return
			gbaromint[gbahookaddr[hookctr]/4+1]=0xE51FF004; //ldr r15,traineraddr
			gbaromint[gbahookaddr[hookctr]/4+2]=realgbaend+0x8000004+hookctr*SIZEOFHOOKJUMP*4; //traineraddr
			trainerintptr+=SIZEOFHOOKJUMP;
*/ //end newly commented out

			gbaromint[gbahookaddr[hookctr]/4]=0xE92D8000; //push r15
			gbaromint[gbahookaddr[hookctr]/4+1]=0xE51FF004; //ldr r15,traineraddr
			gbaromint[gbahookaddr[hookctr]/4+2]=realgbaend+0x8000004+hookctr*SIZEOFHOOKJUMP*4; //traineraddr
			trainerintptr+=SIZEOFHOOKJUMP;


			//}
		}

		if (vblankcheck == 1) {
			copyint(trainerint+trainerintptr,vblankint,6);
			trainerintptr+=6;
		}

		if (excycles > 1) {
			copyint(trainerint + trainerintptr, execint, 10);
			*(trainerint + trainerintptr + 2) |= excycles;
			*(trainerint + trainerintptr + 9) = freeram;
			trainerintptr += 10;			
		}
		if (edstruct.wantenable == 3) {
			copyint(trainerint + trainerintptr, edenint, 19);
			*(trainerint + trainerintptr + 17) = freeram;
			*(trainerint + trainerintptr + 18) = (edstruct.enablekey << 16) | edstruct.disablekey;
			trainerintptr += 19;
		}
		if (edstruct.wantenable == 1) {
			copyint(trainerint + trainerintptr, eddisint, 19);
			*(trainerint + trainerintptr + 17) = freeram;
			*(trainerint + trainerintptr + 18) = (edstruct.enablekey << 16) | edstruct.disablekey;
			trainerintptr += 19;
		}
		if (slomostruct.wantslomo == 1) {
			copyint(trainerint + trainerintptr, slomoint, 35);
			trainerintptr += 35;
			*(trainerint + trainerintptr - 2) = freeram;
			*(trainerint + trainerintptr - 1) = (slomostruct.slowdownkey << 16) | slomostruct.speedupkey;
		}
		if (wantmenu == 1) {
			copyint(trainerint + trainerintptr, trainerigmint, 7);
			*(trainerint + trainerintptr + 4) |= cheatintlen + 6; ////////////////////////////////
			trainerintptr += 7;
		}
		//HexMessage(cheatintlen,"cheatintlen");
		if (cheatintlen > 0) {
			copyint(trainerint + trainerintptr, mycheatint, cheatintlen);
			trainerintptr += cheatintlen;
		}
		else {
			*(trainerint + trainerintptr) = 0xE12FFF1E;
			trainerintptr++;
		}

		/*
		FILE *cheattest=fopen("cheat.bin","wb");
		fwrite(mycheatint,1,cheatintlen*4,cheattest);
		fclose(cheattest);
		*/


		int savejump = 0;
		if (wantmenu==1) {
			#ifdef DEVING
				sprintf(tempchar,"Menu placed at 0x%X - trainerintptr = 0x%X",0x8000000+realgbaend+4+trainerintptr*4,trainerintptr*4);
// 				AddList(hSTATUSLIST,tempchar);
// 				ScrollEnd(hSTATUSLIST);
			#endif
			//menupatch=*(gbaromint+0x30);
			//*(gbaromint+0x30)=0xEA000000|((realgbaend+4)/4+trainerintptr)-0x32; //0x32;
			savejump=*gbaromint;
			*gbaromint=0xEA000000|(((realgbaend+4)/4+trainerintptr)-2);
		}

		copyint(gbaromint+(realgbaend+4)/4,trainerint,trainerintptr);
		//free(trainerint);

		if (wantmenu==1) {
            
            int searchptr=0;
			int menupatchoffset=0;
			for (int searchptr=0; searchptr<0x200; searchptr++) {
				if (*(temptrainermenuint+searchptr+1)==0x8000000) {
					menupatchoffset=searchptr;
					break;
				}
			}
			if (wantbg==1) {
				char * bgptr=(char *)temptrainermenuint;
				bgptr+=(*(temptrainermenuint+searchptr+7)&0xffffff)+8;
				memcpy(bgptr,(char *)menubgshort,76800);
			}
			if (wantselect==1) {
				char * selectptr=(char *)temptrainermenuint;
				selectptr+=(*(temptrainermenuint+searchptr+5)&0xffffff)+8;
				memcpy(selectptr,(char *)menuselectshort,6720);
			}
			if (wantfont==1) {
				char * fontptr=(char *)temptrainermenuint;
				fontptr+=(*(temptrainermenuint+searchptr+2)&0xffffff)+4;
				memcpy(fontptr,(char *)menufontshort,216);
			}
			for (int trainerptr=0; trainerptr<9; trainerptr++) {
				*(temptrainermenuint+menupatchoffset+trainerptr+1)=(*(temptrainermenuint+menupatchoffset+trainerptr+1)&0xffffff)+0x8000000+realgbaend+4+trainerintptr*4;
			}
			copyint(gbaromint+((realgbaend+4)/4)+trainerintptr,temptrainermenuint+1,(*trainermenuint)/4); //TRAINERMENULEN/4);
			*(gbaromint+((realgbaend+4)/4)+trainerintptr+2)=0xE1A00000; //menupatch;
			*(gbaromint+((realgbaend+4)/4)+trainerintptr+4)=0x8000000|((savejump&0xffffff)*4+8);
			//*(gbaromint+((realgbaend+4)/4)+trainerintptr+menupatchoffset)=0x8000000+realgbaend+4+trainerintptr*4;
			*(gbaromint+((realgbaend+4)/4)+trainerintptr+menupatchoffset+9)=cheatselectram;
			copyint(gbaromint+((realgbaend+4)/4)+trainerintptr+(*temptrainermenuint)/4,menuint,0x400); //TRAINERMENULEN/4,menuint,0x400);
			/*
			FILE *menufiletest=fopen("menu.bin","wb");
			fwrite(menuint,1,0x1000,menufiletest);
			fclose(menufiletest);
			*/
		}

		if (fileexists(newgbaromname)==1) remove(newgbaromname);
		formatfopenstr(newgbaromname);
		FILE * newgbaromfile=fopen(newgbaromname,"wb");
		if (newgbaromfile) {
			realgbaend=findromend(gbaromint,gbalen+0x20000);
			int byteswritten=fwrite(gbaromint,1,realgbaend+4,newgbaromfile);			
			free(gbaromint);
			int currlen = ftell(newgbaromfile);
			unsigned char padbuff[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
			int bytewritenum = 16 - (currlen % 16);
			if (bytewritenum == 0) bytewritenum = 16;
			int byteswritten2 = fwrite(padbuff, 1, bytewritenum, newgbaromfile);
			fclose(newgbaromfile);
			if ((byteswritten==realgbaend+4) && (byteswritten2 == bytewritenum)) {
// 				AddList(hSTATUSLIST,"The patched game was written successfully!");
// 				ScrollEnd(hSTATUSLIST);
			}
			else {
// 				AddList(hSTATUSLIST,"There was a problem writing the game!");
// 				ScrollEnd(hSTATUSLIST);
			}
		}
		else {
// 			AddList(hSTATUSLIST,"The file could not be written!");
// 			ScrollEnd(hSTATUSLIST);
		}
	}

	
	return 1;
}
