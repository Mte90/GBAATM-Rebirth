// struct RAMDUMPHANDLES {
// 	HWND hlistbox, harm7edit, harm9edit, harm7button, harm9button;
// };

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



