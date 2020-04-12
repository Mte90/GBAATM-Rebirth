#define MAXCHTLINE 4000
#include <stdio.h>
#include <fstream>
#include <cstring>
#define MAXCODELEN 40960

short byteflip(short number) {
return (((number>>8)&0xff)|((number<<8)&0xff00))&0xffff;
}

int byteflipint(int number) {
	return (((number>>24)&0xff)|((number>>8)&0xff00)|((number<<8)&0xff0000)|((number<<24)&0xff000000));
}

int testcht(char * cheatcodechar,char * srchstr) {
	char * tempchar=(char *)malloc(strlen(cheatcodechar)+10);
	char * tempsrch=(char *)malloc(strlen(srchstr)+10);
	memset(tempchar,0,strlen(cheatcodechar)+10);
	memset(tempsrch,0,strlen(srchstr)+10);
	char thischar;
	int charptr=0;
	for (int charptr=0; charptr<strlen(cheatcodechar); charptr++) {
		thischar=*(cheatcodechar+charptr);
		if ((thischar>=0x61) && (thischar<=0x7a)) thischar-=0x20;
		*(tempchar+charptr)=thischar;
	}
	
	for (charptr=0; charptr<strlen(srchstr); charptr++) {
		thischar=*(srchstr+charptr);
		if ((thischar>=0x61) && (thischar<=0x7a)) thischar-=0x20;
		*(tempsrch+charptr)=thischar;
	}
	int match=0;
	if (strstr(tempchar,tempsrch)) match=1;
	free(tempsrch);
	free(tempchar);
	return match;
}


void formatcheats(char * cheatcodechar) {
	char goodcodechar[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',' '};
	char lastchar[]={0,0};
	char thischar[]={0,0};
	int labellast=0;
	char * lastgood;
	char * templine=(char *)malloc(500);
	memset(templine,0,500);
	char * tempchar=(char *)malloc(strlen(cheatcodechar)*5);
	memset(tempchar,0,strlen(cheatcodechar)*5);
	int howmanylines=0;
	strcat(cheatcodechar,"\r\n");
	for (unsigned int strptr=0; strptr<strlen(cheatcodechar); strptr++) {
		thischar[0]=*(cheatcodechar+strptr);

		if (thischar[0]=='\t') thischar[0]='\n';
		if ((thischar[0]==':') || (thischar[0]=='-')) { thischar[0]=' '; }
		if ( ((thischar[0]<0x20) || (thischar[0]>0x7e)) && (thischar[0]!=0xa) && (thischar[0]!=0x9)) { lastchar[0]=thischar[0]; }
		if ((thischar[0]>=0x61) && (thischar[0]<=0x7a)) { thischar[0]-=0x20; }
		
		if (lastchar[0]!=thischar[0]) {
			if ((thischar[0]!=0x9) && (thischar[0]!=0xd)) {
				strcat(templine,thischar);
				lastchar[0]=thischar[0];
			}
			if (thischar[0]==0xa) {
				if (templine[0]!='/') {
					howmanylines++;
					int goodchars=0;
					for (unsigned int charptr=0; charptr<strlen(templine); charptr++) {
						for (unsigned int goodcharptr=0; goodcharptr<strlen(goodcodechar); goodcharptr++) {
							
							if (templine[charptr]==goodcodechar[goodcharptr]) { goodchars++; }
						}
						
					}
					if ((goodchars>=10) && ((strlen(templine)-goodchars)<5)) {
						if (goodchars<20) {
							if (goodchars<15) {
								char tempcpychar[2];
								memset(tempcpychar,0,2);
								char * newtempline=(char *)malloc(strlen(templine)+20);
								//if ((templine[7]!=' ') && (templine[8]!=' ')) {
								//strleft(templine,newtempline,8);
								//sprintf(newtempline+8," %s\0",templine+8);
								//sprintf(templine,"%s",newtempline);
								//}
								memset(newtempline,0,strlen(templine)+7);
								int tempcpyptr=0;
								while((tempcpychar[0]!=' ') && (tempcpyptr<strlen(templine))) {
									tempcpychar[0]=*(templine+tempcpyptr);
									if (tempcpychar[0]!=' ') { strcat(newtempline,tempcpychar); }
									tempcpyptr++;
								}
								if (tempcpyptr==strlen(templine)) {
									tempcpyptr=strlen(templine)-6;
									*(newtempline+strlen(templine)-6)=0;
								}
								strcat(newtempline," ");
								memset(newtempline+strlen(newtempline),0,strlen(templine)+20-strlen(newtempline));
								//IntMessage(strlen(templine),"strlen(templine)");
								//IntMessage(tempcpyptr,"tempcpyptr");
								memset(newtempline+tempcpyptr+1,'0',10-(strlen(templine)-tempcpyptr));
								sprintf(newtempline+strlen(newtempline),"%s",templine+tempcpyptr);
								//if (tempcpyptr==strlen(templine)) {
								//	char badcodestr[60];
								//	sprintf(badcodestr,"Bad code detected: %s",templine);
								//	AddList(hSTATUSLIST,badcodestr);
								//	ScrollEnd(hSTATUSLIST);
								//	templine[0]=0;
								//}
								//else {
									labellast=0;
									sprintf(templine,newtempline);
								//}
								
								free(newtempline);
								
							}
							
							
						}
						
						
						strcat(tempchar,templine);

					}
					
					else {								 //label or bad code
						if (strlen(templine)>0) {
						templine[strlen(templine) - 2] = 0; //-1]=0x0;
						if ((labellast==0) && (howmanylines>1)) { strcat(tempchar,"}\n"); }
						if (labellast==1) { memset(lastgood,0,100); }
						lastgood=tempchar+strlen(tempchar);
						sprintf(tempchar+strlen(tempchar),"[%s] - %d/%d\n{\n",templine,goodchars,strlen(templine));
						labellast=1;


						}
					}
				}
				memset(templine,0,500);
				
			}
			
		}
		else {
			lastchar[0]=thischar[0];
			if ((thischar[0]!=0x20) && (thischar[0]!=0xa)) { strcat(templine,thischar); }
		}
		
	}

	if (labellast==1) *lastgood=0;

	strcat(tempchar,"}\n");
	
	strcpy(cheatcodechar,tempchar);
	free(tempchar);
#ifdef DEVING
	ErrorMessage(cheatcodechar);
#endif
}


void getnextcheatline(char * cheatcodechar,int * chtptr, char * chtline) {
	memset(chtline,0,MAXCHTLINE*sizeof(char));
	while (1) {
		if ((cheatcodechar[chtptr[0]]=='{') /*|| (cheatcodechar[chtptr[0]]=='}')*/ || (cheatcodechar[chtptr[0]]=='\n')) { //add code here possibly for label with '{'?
		chtptr[0]++;
		break;
	}
	char thischtchar[]={0,0};
	thischtchar[0]=cheatcodechar[chtptr[0]];
	if (thischtchar[0]!='\r') strcat(chtline,thischtchar);
	chtptr[0]++;
	}
}

void getnextchtline(char * cheatcodechar,int * chtptr, char * chtline) {
	//IntMessage(chtptr[0], "ctptr");
	memset(chtline, 0, MAXCHTLINE * sizeof(char));
	/*
	char * endline = strstr(cheatcodechar + chtptr[0], "\n");
	if (endline) {
		memcpy(chtline, cheatcodechar, endline - cheatcodechar);
		chtptr[0] += endline - cheatcodechar + 3;
	}
*/
	
	while (1) {
		char thischtchar;
		thischtchar = cheatcodechar[chtptr[0]];
		*(chtline + strlen(chtline)) = thischtchar;
		chtptr[0]++;
		if (thischtchar == '\n') break;
	}
	
}

void trim(char * textchar,char * texttotrim,char * replacechar) {
	char * tempchar=(char *)malloc(MAXCHTLINE*sizeof(char));
	memset(tempchar,0,MAXCHTLINE*sizeof(char));
	char * lastptr=textchar;
	while (1) {
		char * strptr=strstr(lastptr,texttotrim);
		if (strptr) *strptr=0;
		else {
			strcat(tempchar,lastptr);
			break;
		}
		strcat(tempchar,lastptr);
		strcat(tempchar,replacechar);
		lastptr=strptr+strlen(texttotrim);
	}
	memset(textchar,0,strlen(tempchar)+1);
	sprintf(textchar,"%s",tempchar);
	free(tempchar);
}

int testchtline(char * cheatline) {
	char goodcharlist[]={'a','b','c','d','e','f','A','B','C','D','E','F','0','1','2','3','4','5','6','7','8','9',':',','};
	for (int chtptr=0; chtptr<strlen(cheatline); chtptr++) {
		int thismatch=0;
		for (int goodcharptr=0; goodcharptr<strlen(goodcharlist); goodcharptr++) if (*(cheatline+chtptr)==*(goodcharlist+goodcharptr)) thismatch++;
		if (thismatch==0) return 0;
	}
	return 1;
}

int countcommas(char * textchar) {
	int commas=0;
	for (int textptr=0; textptr<strlen(textchar); textptr++) 
		if (*(textchar+textptr)==',') commas++;
	return commas;
}

void removenpc(char * cheatcodechar) {
	if (strlen(cheatcodechar) == 0) return;
	char * tempccptr = cheatcodechar;
	char * tempchtchar=(char *)malloc(MAXCODELEN);
	memset(tempchtchar,0,MAXCODELEN);
	char * tempchtptr=tempchtchar;
	for (int chtptr=0; chtptr<strlen(cheatcodechar); chtptr++) {
		if ((*tempccptr>0x1F) && (*tempccptr<0x80) && (*tempccptr!=0)) {
			*tempchtptr=*tempccptr;
			tempchtptr++;
		}
		else if (*tempccptr==0xA) {
			*tempchtptr=0xD;
			*(tempchtptr+1)=0xA;
			tempchtptr+=2;
		}
		//else {
		//	*tempchtptr='-';
		//}
		//}
		tempccptr++;
	}
	memset(cheatcodechar,0,MAXCODELEN);
	if (strlen(tempchtchar) == 0) sprintf(cheatcodechar, "Cheat Label\0");
	else sprintf(cheatcodechar,"%s\0",tempchtchar);
	free(tempchtchar);
}

void importcht(char * cheatcodechar) {

	char * tempchar=(char *)malloc(MAXCODELEN);
	memset(tempchar,0,MAXCODELEN);

	char * endcht=strstr(cheatcodechar,"GameInfo");
	if (endcht) memset(endcht,0,8);

	int myptr=0;
	int descwaslast=0;

	char * cheatline=(char *)malloc(MAXCHTLINE);
	char * lastdesc=(char *)malloc(MAXCHTLINE);
	char * lastlabel=NULL;
	while (myptr<strlen(cheatcodechar)) {
		getnextchtline(cheatcodechar,&myptr,cheatline);

			//ErrorMessage(cheatline);
		if (testcht(cheatline,"OFF=")==1) continue;
		if (strlen(cheatline)>0) {
			trim(cheatline,"[","");
			trim(cheatline,"]","");
			trim(cheatline,",\r\n","\r\n");
			trim(cheatline,";\r\n","\r\n");
			trim(cheatline,"ON=\r\n","");
			trim(cheatline,"ON=","");
			trim(cheatline,"0=","");
			trim(cheatline,"MAX=","");
			if (*cheatline=='=') trim(cheatline,"=","");
			trim(cheatline,";","\r\n");
			trim(cheatline,"TEXT=","");
			trim(cheatline,"Text=","");
			//removenpc(cheatline);
			//ErrorMessage(cheatline);
			if (strlen(cheatline)>1) sprintf(tempchar+strlen(tempchar),"%s\r\n",cheatline);
		}
	}

	//ErrorMessage(tempchar);

	memset(cheatcodechar,0,MAXCODELEN);
	memcpy(cheatcodechar,tempchar,strlen(tempchar));
	memset(tempchar,0,MAXCODELEN);


	//FILE * testchtfile=fopen("testcht.txt","w");
	//fwrite(cheatcodechar,1,strlen(cheatcodechar),testchtfile);
	//fclose(testchtfile);

	myptr=0;

	while (myptr<strlen(cheatcodechar)) {
		getnextcheatline(cheatcodechar,&myptr,cheatline);
		if (strlen(cheatline)==0) continue;
		if (*cheatline=='=') continue;
		//if (testcht(cheatline,"OFF=")==1) continue;
		char * multimodchar=strstr(cheatline,"=");
		if (multimodchar) {
			if (lastlabel!=NULL) { //(descwaslast==1) {
				*lastlabel=0;
				lastlabel=NULL;
				//descwaslast=0;
			}

			*multimodchar=0;
			sprintf(tempchar+strlen(tempchar),"%s - %s\r\n",lastdesc,cheatline);
			for (int copyptr=0; copyptr<strlen(multimodchar+1); copyptr++) {
				*(cheatline+copyptr)=*(multimodchar+1+copyptr);
			}
			*(cheatline+strlen(multimodchar+1))=0;
		}
		else descwaslast=0;

		int cheat=testchtline(cheatline);
		
		if (cheat==1) {
			trim(cheatline,":",",");
			descwaslast=0;
			char * addrchar=strstr(cheatline,",");
			*addrchar=0;
			addrchar++;
			int addressint=hextoint(cheatline);
			if (addressint==0x400130) {
				sprintf(tempchar+strlen(tempchar),"D0000020 %04X\r\n",hextoint(addrchar)^0xff);
				continue;
			}
			if ((addressint==0) || (addressint>0x41FFFFF)) {
				sprintf(tempchar+strlen(tempchar),"//%08X:%s ;pointer code????\r\n",addressint,addrchar);
				continue;
			}

			if (addressint<=0x3FFFF) addressint|=0x2000000;
			else if ((addressint&0xf000000)==0) addressint=(addressint&0xffff)|0x3000000;

			char * morecommas=strstr(addrchar,",");
			if (!morecommas) {

				switch (strlen(addrchar)) {
					case 1:
					case 2:
						sprintf(tempchar+strlen(tempchar),"3%X %04X\r\n",addressint,hextoint(addrchar));
						break;
					case 3:
					case 4:
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint,hextoint(addrchar));
						break;
					default:
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint,hextoint(addrchar)&0xffff);
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint+2,(hextoint(addrchar)>>16)&0xffff);
				}

			}
			else {
				unsigned int values[1024];
				memset(&values,0,sizeof(int)*1024);
				int commacount=countcommas(addrchar);
				char * tempaddrchar=addrchar;
				char * newaddrchar;
				int size=1;
				for (int commaptr=0; commaptr<commacount+1; commaptr++) {
					newaddrchar=strstr(tempaddrchar,",");
					if (newaddrchar) *newaddrchar=0;
					values[commaptr]=hextoint(tempaddrchar);
					if ((values[commaptr]>0xff) && (size<2)) size=2;
					if (values[commaptr]>0xffff) size=4;
					tempaddrchar=newaddrchar+1;
				}
					
				if (size==1) {
					switch (commacount) {
						case 1:
								sprintf(tempchar+strlen(tempchar),"8%X %02X%02X\r\n",addressint,values[1],values[0]);
								break;
						case 2:
							if ((addressint%2)==0) { //even address to start
								sprintf(tempchar+strlen(tempchar),"8%X %02X%02X\r\n",addressint,values[1],values[0]);
								sprintf(tempchar+strlen(tempchar),"3%X %04X\r\n",addressint+2,values[2]);
							}
							else {
								sprintf(tempchar+strlen(tempchar),"3%X %04X\r\n",addressint,values[0]);
								sprintf(tempchar+strlen(tempchar),"3%X %02X%02X\r\n",addressint+1,values[2],values[1]);
							}
							break;
						case 3:
							sprintf(tempchar+strlen(tempchar),"8%X %02X%02X\r\n",addressint,values[1],values[0]);
							sprintf(tempchar+strlen(tempchar),"8%X %02X%02X\r\n",addressint+2,values[3],values[2]);
							break;
						default:
							int commahalf=((int)((commacount+1)/2));
							sprintf(tempchar+strlen(tempchar),"5%X %04X\r\n",addressint,commahalf);
							int commaptr=0;
							while (commaptr<commahalf) {
									int thisval[]={0,0,0};
									if (commaptr<commahalf) thisval[0]=((values[commaptr*2+1])&0xff)|((values[commaptr*2]&0xff)<<8);
									if (commaptr+1<commahalf) thisval[1]=((values[commaptr*2+3])&0xff)|((values[commaptr*2+2]&0xff)<<8);
									if (commaptr+2<commahalf) thisval[2]=((values[commaptr*2+5])&0xff)|((values[commaptr*2+4]&0xff)<<8);
									sprintf(tempchar+strlen(tempchar),"%04X%04X %04X\r\n",thisval[0],thisval[1],thisval[2]);
									commaptr+=3;
							}
							if ((commacount+1)%2!=0) sprintf(tempchar+strlen(tempchar),"3%X %04X\r\n",addressint+commahalf*2,values[commacount]);
					}
				}
				if (size==2) {
					if (commacount==1) {
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint,values[0]);
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint+2,values[1]);
						continue;
					}

					if (commacount>1) {
						sprintf(tempchar+strlen(tempchar),"5%X %04X\r\n",addressint,commacount+1);
						for (int commactr=0; commactr<commacount+1; commactr+=3) {
							int thisval[]={0,0,0};
							if (commactr<commacount+1) thisval[0]=((values[commactr]>>8)&0xff)|((values[commactr]&0xff)<<8);
							if (commactr+1<commacount+1) thisval[1]=((values[commactr+1]>>8)&0xff)|((values[commactr+1]&0xff)<<8);
							if (commactr+2<commacount+1) thisval[2]=((values[commactr+2]>>8)&0xff)|((values[commactr+2]&0xff)<<8);
							sprintf(tempchar+strlen(tempchar),"%04X%04X %04X\r\n",thisval[0],thisval[1],thisval[2]);
						}
					}

				}


				if (size==4) {
					if (commacount==1) {
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint,values[0]&0xffff);
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint+2,(values[0]>>16)&0xffff);
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint+4,values[1]&0xffff);
						sprintf(tempchar+strlen(tempchar),"8%X %04X\r\n",addressint+6,(values[1]>>16)&0xffff);
						continue;
					}

					if (commacount>1) {
						sprintf(tempchar+strlen(tempchar),"5%X %04X\r\n",addressint,(commacount+1)*2);
						for (int commactr=0; commactr<commacount+1; commactr++) {
							int thisval=((values[commactr]&0xff)<<24)|(((values[commactr]>>8)&0xff)<<16)|((values[commactr]&0xff0000)>>8)|((values[commactr]>>24)&0xff);
							if ((commactr%3)==0) sprintf(tempchar+strlen(tempchar),"%08X ",thisval);
							if ((commactr%3)==1) sprintf(tempchar+strlen(tempchar),"%04X\r\n%04X",(thisval>>16)&0xffff,thisval&0xffff);
							if ((commactr%3)==2) sprintf(tempchar+strlen(tempchar),"%04X %04X\r\n",(thisval>>16)&0xffff,thisval&0xffff);



						}
						if (((commacount+1)%3)==1) sprintf(tempchar+strlen(tempchar),"0000\r\n");
						if (((commacount+1)%3)==2) sprintf(tempchar+strlen(tempchar),"0000 0000\r\n");
					}

				}


			}
		}
		else {
			descwaslast=1;
			lastlabel=tempchar+strlen(tempchar);
			sprintf(tempchar+strlen(tempchar),"%s\r\n",cheatline);
			memset(lastdesc,0,MAXCHTLINE);
			sprintf(lastdesc,"%s",cheatline);
		}
	}
	memset(cheatcodechar,0,MAXCODELEN);
	memcpy(cheatcodechar,tempchar,MAXCODELEN);

	free(tempchar);
	free(cheatline);

// 	AddList(hSTATUSLIST,"CHT format detected -- importing cheats");
// 	ScrollEnd(hSTATUSLIST);
}




int convertraw(char * cheatcodes,unsigned int * cheatcodeint,int wantmenu,int cheatselectram,unsigned int * menuint) {


	//unsigned int myint[]={0xE3A020FF,0xE59F100C,0xE1D100B0,0xE1500002,0x1A000001,0xEA000000,0xDDDDDDDD};

unsigned int bytewrite[]={0xE3A00000,0xE59F1004,0xE5C10000,0xEA000000,0xFFFFFFFF};
unsigned int bigwrite[]={0xE59F000C,0xE59F1004,0xE1C100B0,0xEA000001,0xFFFFFFFF,0xEEEEEEEE};

unsigned int loadramreg[]={0xE59FB000,0xEA000000,cheatselectram};
//unsigned int ramtest[]={0xE5DBA000,0xE35A0000,0x0A000000};
unsigned int ramtest[]={0xE5DB0000,0xE59F1008,0xE0100001,0xA000002,0xEA000000};
int intcounter=0;
int cheatcounter=0;

#define maxaddr 300

int menujmp=0;
int jumpwidth=0;
int whichbit=-1;
char * tempaddr=(char *)malloc(9*sizeof(char));

char tempchar[600];

	int asmloop=0;
	int asmlabel=0;
	int labellast=0;
			int * cheatptr=(int *)malloc(2*sizeof(int));
			char * cheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
			char * lastcheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
			cheatptr[0]=0;
			cheatline[0]=0;


			if (wantmenu==1) {
				copyint(cheatcodeint+intcounter,loadramreg,3);
				intcounter+=3;
			}

			while (cheatptr[0]<strlen(cheatcodes)) {
				if (strlen(cheatline)>0) { strcpy(lastcheatline,cheatline); }
				getnextcheatline(cheatcodes,cheatptr,cheatline);

				if ((cheatline[0]=='[') && (wantmenu==1)) {
					int menuoffset=cheatcounter*7+1;
					int menuitemlength=strstr(cheatline,"]")-cheatline-2;
					if (menuitemlength>24) menuitemlength=24;
					memcpy(menuint+menuoffset,cheatline+1,menuitemlength);
					//memset(menuint+strlen(cheatline)-2,0,28-(strlen(cheatline)-2));
				}
				if (cheatline[0]=='[') {
					cheatcounter++;
					if (wantmenu==1) {
					whichbit++;
					if (whichbit==8) {
						*(cheatcodeint+intcounter)=0xE28BB001;
						intcounter++;
						whichbit=0;
					}
					}

				}

				if ((cheatline[0]=='}') && (wantmenu==1)) {
					*menuint=0x1c00|cheatcounter;
					*(cheatcodeint+menujmp)+=jumpwidth;
					menujmp=0;
					jumpwidth=0;

				}

				if ((cheatline[0]=='[') || (cheatline[0]=='{') || (cheatline[0]=='}')) {
						//add some labeling code here if needed
						cheatline[0]=0;
					}
				if (strlen(cheatline)!=0) {

					char templongchar[9];
					memset(templongchar,0,9);
					strright(cheatline,templongchar,8);

					int decval=hextoint(templongchar);

						strleft(cheatline,tempaddr,8);


							int tempdec=hextoint(tempaddr);
							sprintf(tempaddr,"%X",(tempdec&0xfffffff));

					if (decval<=0xff) {


						if (wantmenu==1) {
						copyint(cheatcodeint+intcounter,ramtest,5);
						*(cheatcodeint+intcounter+5)=(1<<whichbit);
						//*(cheatcodeint+intcounter)|=cheatcounter-1;
						//*(cheatcodeint+intcounter+2)|=4;
						if (menujmp==0) {
							menujmp=intcounter+3;
							jumpwidth=4;
						}
						else {
							jumpwidth+=5;
						}
						intcounter+=6;
						}

						copyint(cheatcodeint+intcounter,bytewrite,5);
						*(cheatcodeint+intcounter)|=decval;
						*(cheatcodeint+intcounter+4)=tempdec;
						intcounter+=5;

					}
					else {

						if (wantmenu==1) { /////////////////////////////////////////////////////might need to fix offsets
						copyint(cheatcodeint+intcounter,ramtest,5);
						*(cheatcodeint+intcounter+5)=(1<<whichbit);
						//*(cheatcodeint+intcounter)|=cheatcounter-1;
						//*(cheatcodeint+intcounter+2)|=5;
						if (menujmp==0) {
							menujmp=intcounter+2;
							jumpwidth=5;
						}
						else {
							jumpwidth+=6;
						}
						intcounter+=3;
						}
						

						copyint(cheatcodeint+intcounter,bigwrite,6);
						*(cheatcodeint+intcounter+4)=tempdec;
						*(cheatcodeint+intcounter+5)=decval;

							if (decval>0xffff) *(cheatcodeint+intcounter+2)=0xE5810000;

							intcounter+=6;
					}



				}

			}

										*(cheatcodeint+intcounter)=0xE12FFF1E;
										intcounter++;

										*(menuint+cheatcounter*7+1)=0xEFBEADDE;

//IntMessage(cheatcounter,"cheatcounter");

return intcounter;
}









int convertcb(char * cheatcodes,unsigned int * cheatcodeint,int wantmenu,int cheatselectram,unsigned int * menuint) {
	//ErrorMessage(cheatcodes);
	
	unsigned int iftopbyte[]={0xE3A02000,0xE59F100C,0xE1D100B0};
	unsigned int superint[]={0xE59F1014,0xE28F2014,0xE0D200B2,0xE0C100B2,0xE2533001,0x1AFFFFFB,0xEA000000};
	unsigned int slideint[]={0xE1DF21BE,0xE1DF31BC,0xE1DF41BA,0xE08100B2,0xE0800004,0xE2533001,0x1AFFFFFB,0xEA000002};
	//unsigned int dpadint[]={0xE3A01301,0xE5910130,0xE1500002,0x1A000000};
	unsigned int dpadint[]={0xE3A01301,0xE5911130,0xE3A00C03,0xE28000FF, 0xE0010000,0xE1500002, 0x1A000000};
	
	unsigned int loadramreg[]={0xE59FB000,0xEA000000,cheatselectram};
	//unsigned int ramtest[]={0xE5DBA000,0xE35A0000,0x0A000000};
	unsigned int ramtest[]={0xE5DB0000,0xE59F1008,0xE0100001,0xA000000,0xEA000000};
	
	int intcounter=0;
	int menujumppatch = 0;
	int jumppatches[10];
	int numpatches=0;
	int superptr=0;
	int slideval=-1;
	int whichbit=-1;
	int cheatcounter=0;
	int mastercodeon=0;
	
#define maxaddr 300
	
	char * tempaddr=(char *)malloc(9*sizeof(char));
	char * tempstr=(char *)malloc(50*sizeof(char));
	char * lvalstr=(char *)malloc(4*sizeof(char));
	char * templongchar=(char *)malloc(9*sizeof(char));
	
	int labellevel=0;
	int encryptionon=0;
	int slideon=0;
	int superon=0;
	int conditionalon=0;
	int conditionallast = 0;
	int nextconditional = 0;
	char tempchar[600];
	
	int labellast = 1; //=0;
	int * cheatptr=(int *)malloc(2*sizeof(int));
	char * cheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
	char * lastcheatline=(char *)malloc(MAXCHTLINE*sizeof(char));
	cheatptr[0]=0;
	cheatline[0]=0;
	
	
	if (wantmenu==1) {
		copyint(cheatcodeint+intcounter,loadramreg,3);
		intcounter+=3;
	}
	
	while (cheatptr[0]<strlen(cheatcodes)) {
		if (strlen(cheatline)>0) { strcpy(lastcheatline,cheatline); }
		getnextcheatline(cheatcodes,cheatptr,cheatline);
		
		if (cheatline[0] == '[') {
			
			labellast = 1;
			
			if (wantmenu == 1) {
				whichbit++;
				if (whichbit==8) {
					*(cheatcodeint+intcounter)=0xE28BB001;
					intcounter++;
					whichbit=0;
				}
				
				//copyint(cheatcodeint+intcounter,ramtest,5);
				//*(cheatcodeint+intcounter+5)=(1<<whichbit);
				
				//jumppatches[numpatches]=intcounter+3;
				//numpatches++;
				menujumppatch = intcounter + 3;
				
				//memset(tempchar,0,600);
				//memcpy(tempchar,cheatline+1,strstr(cheatline,"]")-cheatline-2);
				//if (strlen(tempchar)>24) *(tempchar+24)=0;
				
				//int menuoffset=cheatcounter*7+1;
				//memcpy(menuint+menuoffset,tempchar,strlen(tempchar)); //cheatline+1,strstr(cheatline,"]")-cheatline-2);
				
				int menuoffset=cheatcounter*7+1;
				int menuitemlength=strstr(cheatline,"]")-cheatline-1; //2;
				if (menuitemlength>24) menuitemlength=24;
				memcpy(menuint+menuoffset,cheatline+1,menuitemlength);
				
				copyint(cheatcodeint+intcounter,ramtest,5);
				*(cheatcodeint+intcounter+5)=(1<<whichbit);
				intcounter+=6;
				conditionalon=2;
				conditionallast = 1;
				
			}
			else menujumppatch = 0;
			
			cheatcounter++;
			
		}
		
		if ((cheatline[0] == '}') && (wantmenu == 1)) {
			*menuint = 0x1c00 | cheatcounter;
			*(cheatcodeint + menujumppatch) |= (intcounter - menujumppatch - 2);
			//*(cheatcodeint+menujmp)+=jumpwidth;
			//menujmp=0;
			//jumpwidth=0;
			
		}
		
		nextconditional = 0;
		
		switch (cheatline[0]) {
		case '7':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'F':
				//ErrorMessage(cheatline);
				//IntMessage(conditionallast, "conditionallast");
				//IntMessage(labellast, "labellast");
				//IntMessage(encryptionon, "encryptionon");
			
			if ((conditionallast == 0) && (labellast == 0) && (encryptionon == 0)) {
				conditionalon = 2; //change to 2?
				nextconditional = 1;
			}
			break;
		default:
			break;
		}
		
		
		if ((cheatline[0] == '}') || (nextconditional == 1)) {
			//IntMessage(intcounter, "intcounter");
			if (conditionalon != 0) {
				labellast = 1;
				conditionalon = 0;
				nextconditional = 0;
				
				for (int jumpctr = 0; jumpctr < numpatches; jumpctr++) {
					//HexMessage(jumppatches[jumpctr] * 4, "Jump patch offset");
					*(cheatcodeint + jumppatches[jumpctr]) |= (intcounter - jumppatches[jumpctr] - 2);
				}
				
				numpatches = 0;
				
			}
			
		}
		
		conditionallast = 0;
		
		
		if ((cheatline[0] == '[') || (cheatline[0] == '{')) { // || (cheatline[0] == '}')) {
				//ErrorMessage(cheatline);
			
			labellast = 1; //might need to take out
			
			//add some labeling code here if needed
			cheatline[0] = 0;
			mastercodeon = 0;
		}
		else labellast = 0;
		
		//if (((cheatline[0] >= 'A') && (cheatline[0] <= 'F')) || ((cheatline[0] >= '0') && (cheatline[0] <= '9'))) labellast = 0;
		
		if (mastercodeon == 1) continue; //cheatline[0]=0;
		
		if (superon <= 0) {
			if ((((cheatline[0] == '0') && (slideon == 0)) || (cheatline[0] == '1')) && (encryptionon == 0)) {
				intcounter-=6;
				conditionalon=0;
				whichbit--;
				mastercodeon=1;
				cheatcounter--;
				cheatline[0]=0;
			}
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
				unsigned char codebuffer[8]={(tempaddress&255),((tempaddress>>8)&255),((tempaddress>>16)&255),((tempaddress>>24)&255),(tempvalue&255),((tempvalue>>8)&255),0,0};
				cheatsCBADecrypt(codebuffer);
				int * codebuffint=(int *)codebuffer;
				tempaddress=*codebuffint;
				short * codebuffshort=(short *)codebuffer;
				tempvalue=*(codebuffint+1);
				sprintf(cheatline,"%08X %08X",tempaddress,tempvalue);
				//ErrorMessage(cheatline);
			}
			
			
			
			//	* CodeBreaker codes types: (based on the CBA clone "Cheatcode S" v1.1)
			//	*
			//	* 0000AAAA 000Y - Game CRC (Y are flags: 8 - CRC, 2 - DI)
			//	* 1AAAAAAA YYYY - Master Code function (store address at ((YYYY << 0x16)
			//	*                 + 0x08000100))
			//	* 2AAAAAAA YYYY - 16-bit or
			//	* 3AAAAAAA YYYY - 8-bit constant write
			//	* 4AAAAAAA YYYY - Slide code
			//	* XXXXCCCC IIII   (C is count and I is address increment, X is value incr.)
			//	* 5AAAAAAA CCCC - Super code (Write bytes to address, 2*CCCC is count)
			//	* BBBBBBBB BBBB 
			//	* 6AAAAAAA YYYY - 16-bit and
			//	* 7AAAAAAA YYYY - if address contains 16-bit value enable next code
			//	* 8AAAAAAA YYYY - 16-bit constant write
			//	* 9AAAAAAA YYYY - change decryption (when first code only?)
			//	* AAAAAAAA YYYY - if address does not contain 16-bit value enable next code
			//	* BAAAAAAA YYYY - if 16-bit value at address  <= YYYY skip next code
			//	* CAAAAAAA YYYY - if 16-bit value at address  >= YYYY skip next code
			//	* D00000X0 YYYY - if button keys ... enable next code (else skip next code)
			//	* EAAAAAAA YYYY - increase 16/32bit value stored in address
			//	* FAAAAAAA YYYY - if 16-bit value at address AND YYYY = 0 then skip next code
			memset(tempaddr,0,9);
			strleft(cheatline,tempaddr,8);
			int tempdec=hextoint(tempaddr);
			
			memset(templongchar,0,9);
			strright(cheatline,templongchar,8);
			int decval=hextoint(templongchar)&0xffff;
			
			if (superon > 0) {
				
				if ((superon % 4) == 0) { //if (((superon/6)%2)==0) {
					*(cheatcodeint + intcounter) = byteflipint(tempdec); //byteflipint(((tempdec&0xffff0000)>>16)|((tempdec&0xffff)<<16));
					//HexMessage(*(cheatcodeint+intcounter), "byte flipped value");
					intcounter++;
					
					if (superon > 4) {
						*(cheatcodeint + intcounter) = (((decval & 0xff00) >> 8) | ((decval & 0xff) << 8));
						intcounter ++;
					}
				}
				else {
					//HexMessage(*(cheatcodeint+intcounter-1), "oldval");
					*(cheatcodeint + intcounter - 1) = byteflipint(((((*(cheatcodeint + intcounter - 1) & 0xff) << 8) | ((*(cheatcodeint + intcounter - 1) & 0xff00) >> 8)) << 16) | ((tempdec & 0xffff0000) >> 16)); //byteflipint(>>16)0x7788<<16
					
					if (superon > 4) {
						*(cheatcodeint + intcounter) = byteflipint(((decval & 0xffff)) | ((tempdec & 0xffff) << 16));
						//HexMessage(*(cheatcodeint+intcounter), "byte flipped value");
						intcounter++;
					}
				}
				
				
				//superon-=6;
				
				superon -= 6; //3;
				
				*(cheatcodeint + superptr) = 0xea000000 | (intcounter - superptr - 2);
				//if (superon==0) conditionalon--;
				if (superon > 0) { 
				}
				else {
					
					//intcounter--;
					
					
					labellast = 1;
				}
				//cheatline[0] = 0;
				
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
			
			if (cheatline[0]=='4') {
				
				*(cheatcodeint+intcounter)=0xE1DF02B4;
				slideval=decval;
				
				
				*(cheatcodeint+intcounter+1)=0xE59F101C;
				*(cheatcodeint+intcounter+10)=(tempdec&0xfffffff);
				
				slideon=1;
				cheatline[0]=0;
				continue;
			}
			
			if (cheatline[0] == '5') {
				if (decval <= 0xff) {
					*(cheatcodeint + intcounter) = (0xE3A03000 | decval);
				}
				else {
					*(cheatcodeint + intcounter) = 0xE59F3018;
				}
				
				superon = decval * 2;
				copyint(cheatcodeint+intcounter + 1, superint, 7);
				*(cheatcodeint + intcounter + 8) = (tempdec & 0xfffffff);
				superptr = intcounter + 7; //7;
				
				if (decval <= 0xff) {
					intcounter += 9; //9;
				}
				else {
					*(cheatcodeint + intcounter + 9) = (decval & 0xffff);
					intcounter += 10; //10;
					superptr++;
				}
				
				cheatline[0] = 0;
				
				continue;
				
			}
			
			if ((cheatline[0]=='9') && (encryptionon==0)) {
				unsigned int seed[8];
				cheatsCBAParseSeedCode(tempdec,decval,seed);
				cheatsCBAChangeEncryption(seed);
				encryptionon=1;
				mastercodeon=1;
				//intcounter-=6;
				conditionalon=0;
				whichbit--;
				//numpatches--;
				cheatcounter--;
				cheatline[0]=0;
			}
			
			
			if ((cheatline[0]=='2') || (cheatline[0]=='6') || (cheatline[0]=='E')) {
				
				if (decval<=0xff) {
					
					
					*(cheatcodeint+intcounter)=(0xE3A02000|decval);
					
				}
				else {
					
					
					*(cheatcodeint+intcounter)=0xE59F2014;
					
				}
				
				
				*(cheatcodeint+intcounter+1)=0xE59F100C;
				*(cheatcodeint+intcounter+2)=0xE1D100B0;
				
				if (cheatline[0]=='2') {
					*(cheatcodeint+intcounter+3)=0xE1800002;
				}
				if (cheatline[0]=='6') {
					*(cheatcodeint+intcounter+3)=0xE0000002;
				}
				if (cheatline[0]=='E') {
					*(cheatcodeint+intcounter+3)=0xE0800002;
				}
				
				
				
				*(cheatcodeint+intcounter+4)=0xE1C100B0;
				if (decval<=0xff) {
					*(cheatcodeint+intcounter+5)=0xEA000000;
				}
				else {
					*(cheatcodeint+intcounter+5)=0xEA000001;
				}
				
				
				*(cheatcodeint+intcounter+6)=(tempdec&0xfffffff);
				
				intcounter+=7;
				
				if (decval>0xff) {
					*(cheatcodeint+intcounter)=decval;
					intcounter++;
				}
				
				
				
			}
			
			if ((cheatline[0]=='3') || (cheatline[0]=='8')) {
				
				
				if ((cheatline[0]=='3') || (decval<=0xff)) {
					
					*(cheatcodeint+intcounter)=(0xE3A00000|decval);
					
				}
				else {
					
					
					*(cheatcodeint+intcounter)=0xE59F000C;
					
				}
				
				*(cheatcodeint+intcounter+1)=0xE59F1004;
				
				if (cheatline[0]=='3') {
					
					*(cheatcodeint+intcounter+2)=0xE5C10000;
					*(cheatcodeint+intcounter+3)=0xEA000000;
					*(cheatcodeint+intcounter+4)=(tempdec&0xfffffff);
					
					intcounter+=5;
					
				}
				else {
					
					*(cheatcodeint+intcounter+2)=0xE1C100B0;
					*(cheatcodeint+intcounter+3)=0xEA000001;
					*(cheatcodeint+intcounter+4)=(tempdec&0xfffffff);
					*(cheatcodeint+intcounter+5)=(decval&0xffff);
					
					intcounter+=6;
				}
				
				
			}
			
			if ((cheatline[0] == '7') || (cheatline[0] == 'A') || (cheatline[0] == 'B') || (cheatline[0] == 'C') || (cheatline[0] == 'F')) {
				
				//ErrorMessage(cheatline);
				if (decval<=0xff) {
					
					copyint(cheatcodeint+intcounter,iftopbyte,3);
					*(cheatcodeint+intcounter)|=decval;
					
				}
				else {
					
					copyint(cheatcodeint+intcounter,iftopbyte,3);
					*(cheatcodeint+intcounter)=0xE59F2014;
					
				}
				
				if (cheatline[0]=='F') {
					if (decval>0xff) {
						*(cheatcodeint+intcounter)+=4;
					}
					*(cheatcodeint+intcounter+1)+=4;
				}
				
				intcounter+=3;
				
				if (cheatline[0]=='F') {
					*(cheatcodeint+intcounter)=0xE0000002;
					intcounter++;
				}
				
				*(cheatcodeint+intcounter)=0xE1500002;
				intcounter++;
				
				
				//bne = 1a, beq = 0a, ble = da, bge = aa
				
				if (cheatline[0] == '7') {
					
					
					*(cheatcodeint + intcounter) = 0x1a000000;
				}
				if (cheatline[0]=='A') {
					
					
					*(cheatcodeint+intcounter)=0x0a000000;
					
					
				}
				if (cheatline[0]=='B') {
					
					
					*(cheatcodeint+intcounter)=0xda000000;
					
				}
				if (cheatline[0]=='C') {
					
					
					*(cheatcodeint+intcounter)=0xaa000000;
					
					
				}
				if (cheatline[0]=='F') { //might need to fix
					
					if (decval>0xff) { *(cheatcodeint+intcounter-4)+=4; }
					*(cheatcodeint+intcounter-3)+=4;
					*(cheatcodeint+intcounter+1)=0x1a000000;
					intcounter++;
					
				}
				
				
				jumppatches[numpatches] = intcounter;
				if (cheatline[0] == 'F') jumppatches[numpatches]++;
				numpatches++;
				
				
				intcounter++;
				
				*(cheatcodeint + intcounter) = 0xEA000000;
				if (decval > 0xff) *(cheatcodeint + intcounter) |= 1;
				
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
				}
				else {
					
					*(cheatcodeint + intcounter) = 0xE59F201C; //7 0; //4;
				}
				
				copyint(cheatcodeint + intcounter + 1, dpadint, 7); //5);
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
		
		
		
		*(cheatcodeint+intcounter) = 0xE12FFF1E;
		intcounter++;
		
		*(menuint + cheatcounter * 7 + 1) = 0xEFBEADDE;		
		return intcounter;
}

void parsecheat(char * filename) {
    FILE * cheatfile=fopen(filename,"r");
    if (cheatfile) {
        fseek(cheatfile,0,SEEK_END);
        int cheatlen=ftell(cheatfile);
        rewind(cheatfile);
        char * cheatchar=(char *)malloc(MAXCODELEN);
        memset(cheatchar,0,MAXCODELEN);
        fread(cheatchar,1,cheatlen,cheatfile);
        fclose(cheatfile);
        strcat(cheatchar,"\r\n");
        if (testcht(cheatchar,"[gameinfo]")==1) importcht(cheatchar);
        else removenpc(cheatchar);
        free(cheatchar);
    }
}
