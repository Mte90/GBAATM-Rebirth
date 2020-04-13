#include <QString>
//#include "core/GBAATMres.h"
#include "functions.h"

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
