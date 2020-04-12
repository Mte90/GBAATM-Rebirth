
int addresstest(char * addrtotest, char * asmaddresses, ADDRESSSTRUCT addressstruct) {
	//IntMessage(*addressstruct.asmaddr,"address test");
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

/*
int shortvaluetest(char * hvaltotest) {
	char * asmhvalstr=(char *)malloc(3*sizeof(char));
	int hvaltest=-1;
	int hvaldec=hextoint(hvaltotest);
	for (int whichhval=0; whichhval<asmhvalue; whichhval++) {
		if (oldasmhvalues[whichhval]==hvaldec) { hvaltest=whichhval; }	  
	}
	
	if (hvaltest==-1) {
		hvaltest=asmhvalue;
		oldasmhvalues[asmhvalue]=hvaldec;
		strcat(asmhvals,"hval");
		sprintf(asmhvalstr,"%d",hvaltest);
		strcat(asmhvals,asmhvalstr);
		strcat(asmhvals, ": .short 0x");
		sprintf(hvaltotest,"%X",hvaldec);
		strcat(asmhvals,hvaltotest);
		strcat(asmhvals,"\n");
		asmhvalue++;
	}
	free(asmhvalstr);
	return hvaltest;
}
*/
