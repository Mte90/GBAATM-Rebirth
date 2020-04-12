#include <QString>
#include "core/GBAATMres.h"
#include "functions.h"
int new_hextoint(QString hexstr) {
    int pos=0;
    hexstr = hexstr.toLocal8Bit().data();
    char hexmap[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','a','b','c','d','e','f'};
    int returnval=-1;
    while (hexstr[pos]!=0) {
        for(int i=0; i<22; i++) {
        if (hexstr[pos]==hexmap[i]) {
            if (returnval==-1) { returnval=0; }
            returnval<<=4;
                if (i<16) {
                    returnval|=i;
                } else {
                    returnval|=(i-6);
                }
            }
        }
        pos++;
    }
    return returnval;
}

void new_getpathfromfilename(char * filename, char * path) {
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

int addresstest(char * addrtotest, char * asmaddresses, ADDRESSSTRUCT addressstruct) {
    char * asmaddrstr=(char *)malloc(3*sizeof(char));
    int addrtest=-1;
    int addrdec=hextoint(addrtotest);
    for (int whichaddr=0; whichaddr<*addressstruct.asmaddr; whichaddr++) {
        if (*(addressstruct.oldasmaddrs+whichaddr)==addrdec) { addrtest=whichaddr; }
    }

    if ((addrtest==-1)) {// && (hextoint(addrtotest)>0xffffff)) {
        addrtest=*addressstruct.asmaddr;
        *(addressstruct.oldasmaddrs+*addressstruct.asmaddr)=addrdec;
        strcat(asmaddresses,"address");
        sprintf(asmaddrstr,"%d",addrtest);
        strcat(asmaddresses,asmaddrstr);
        strcat(asmaddresses, ": .long 0x");
        sprintf(addrtotest,"%X",addrdec);
        strcat(asmaddresses,addrtotest);
        strcat(asmaddresses,"\n");
        *addressstruct.asmaddr=*addressstruct.asmaddr+1;
    }
    free(asmaddrstr);
    return addrtest;
}

int longvaluetest(char * lvaltotest, char * asmlvals, LVALSTRUCT lvalstruct) {
    //IntMessage(*lvalstruct.asmlvalue,"lval test");
    char * asmlvalstr=(char *)malloc(3*sizeof(char));
    int lvaltest=-1;
    int lvaldec=hextoint(lvaltotest);
    for (int whichlval=0; whichlval<*lvalstruct.asmlvalue; whichlval++) {
        if (*(lvalstruct.oldasmlvalues+whichlval)==lvaldec) { lvaltest=whichlval; }
    }

    if (lvaltest==-1) {
        lvaltest=*lvalstruct.asmlvalue;
        *(lvalstruct.oldasmlvalues+*lvalstruct.asmlvalue)=lvaldec;
        strcat(asmlvals,"lval");
        sprintf(asmlvalstr,"%d",lvaltest);
        strcat(asmlvals,asmlvalstr);
        strcat(asmlvals, ": .long 0x");
        sprintf(lvaltotest,"%X",lvaldec);
        strcat(asmlvals,lvaltotest);
        strcat(asmlvals,"\n");
        *lvalstruct.asmlvalue=*lvalstruct.asmlvalue+1;
    }
    free(asmlvalstr);
    return lvaltest;
}
