#include <QString>
//#include "core/GBAATMres.h"
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
