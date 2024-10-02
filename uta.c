#include <stdio.h>
#include <string.h>

#define	AUTO 2
#define	GAL 1
#define	ENG 0
#define	TOKENS " ,?;:!@#$%^&*()_=+\"[]`~/\\1234567890.|\n"
char *modes[] =	{"ENG","GAL","AUTO"};
char *tmode[] =	{"Verbose","Silent","Subtle"};
#define	PROG "UTA: Universal Translator	Assistant"
#define	VERS "2"
#define	LVL "4"
#define	DATE "December 7,2009"
#define	DFLT_DICT "uta.lng"
#define	MAXWDS 8000
/*Global Variable Dictionary*/

char bad[200];
char input[5000],OUTP[5000],*g,*e,*i;
char *voc[MAXWDS][2];
char curfile[200];
int VERBOSE = 0;

FILE *normout;
FILE *normin;

/*char *voc[2000][2][100];*/
int silent = 0;	 /*show	commercial if not -q*/
int wcount=0;
int mode=AUTO;
int done=0;
int CASE=1; /*default case sensitive*/
char *cases[] =	{"insensitive","sensitive"};
char *blank = "	";
/*for keeping stats*/
int tstat = 0; /*translated*/
int wstat = 0; /*tried*/

/*ancillary functions*/


docommercial(){
  if (silent ==	0){
	printf ("\n\nUniversal Translator Assistant %s.%s\nJoel	Peter Anderson,	%s\n",VERS,LVL,DATE);
	printf ("Visit the UTA Project at \n\thttp://www.mrklingon.org\n\n");
  }
}
jcmp (char *a, char *b)	{
	if (CASE == 0){
		return (strcasecmp(a,b));
	}else{
		return (strcmp(a,b));
	}
}
/*
 _   _ ___  __ _  __ _	___
| | | /	__|/ _`	|/ _` |/ _ \
| |_| \__ \ (_|	| (_| |	 __/
 \__,_|___/\__,_|\__, |\___|
		 |___/
*/
void usage(){
  printf ("USAGE:\n");
  printf ("  ? usage note\n");
  printf ("  = current mode\n");
  printf ("  <file read	file for language\n");
  printf ("  >file write file for language\n");
  printf ("  +worda wordb - add	worda/wordb pair\n");
  printf ("  ! switch mode\n");
  printf ("  *case - toggle case sensitive\n");
  printf ("  *quit - exit\n");
  printf ("  *lcars - toggles verbose LCARS style output\n");
  printf ("\n");
}

/*
  __ _		 _	       _
 / _(_)_ __   __| | __ _  __ _|	|
| |_| |	'_ \ / _` |/ _`	|/ _` |	|
|  _| |	| | | (_| | (_|	| (_| |	|
|_| |_|_| |_|\__,_|\__,	|\__,_|_|
		   |___/

*/
char *findgal(char *eng){
	int x;
	for (x=0;x<wcount;x++){
		if (!jcmp (voc[x][ENG],eng)){
			return	(voc[x][GAL]);
		}
	}

	sprintf(bad,"[%s]",eng);

	return bad;
}

/*
  __ _		 _
 / _(_)_ __   __| | ___	_ __   __ _
| |_| |	'_ \ / _` |/ _ \ '_ \ /	_` |
|  _| |	| | | (_| |  __/ | | | (_| |
|_| |_|_| |_|\__,_|\___|_| |_|\__, |
			      |___/

*/
char *findeng(char *gal){
	int x;
	for (x=0;x<wcount;x++){
		if (!jcmp (voc[x][GAL],gal)){
			return	(voc[x][ENG]);
		}
	}

	sprintf(bad,"[%s]",gal);

	return bad;
}

/*
 _
| |_ _ __ __ _ _ __  ___
| __| '__/ _` |	'_ \/ __|
| |_| |	| (_| |	| | \__	\
 \__|_|	 \__,_|_| |_|___/

*/
char *trans(char *intext, int MODE){
  char *g = findgal(intext);
  char *e = findeng(intext);

  if ((strlen(intext)==0)|| (*intext ==	' ')) {
    return blank;
  }

  switch (MODE){
  case ENG:
    return g;
    break;

  case GAL:
    return e;
    break;

  default:
    if (g!= bad){
      return g;
    }
    return e;
  }
}


/*
		    _	     _ _      _
 _ __ ___  __ _	 __| |	  __| (_) ___| |_
| '__/ _ \/ _` |/ _` |	 / _` |	|/ __| __|
| | |  __/ (_| | (_| |	| (_| |	| (__| |_
|_|  \___|\__,_|\__,_|___\__,_|_|\___|\__|
		    |_____|

*/
int read_dict(char *filename){
	FILE *RD;
	int x;
	char *ecop,*gcop;
	char FILENM[1000];

	strcpy (curfile, filename);

	for (x=0;x<wcount;x++){
	  free(voc[x][GAL]);
	  free(voc[x][ENG]);
	}

	wcount = 0;
	strcpy (FILENM,FILEHOME);
	strcat (FILENM,filename);

	if (RD = fopen(FILENM,"r")) {

		while ((	fgets (input,2000,RD) )&&(wcount<MAXWDS)){

			e = strchr(input,'\"');

			e++; /*now we have the english word*/

			g = strchr(e,'\"');

			*g = (char)0; /*null out end of	eng word*/

			g++;

			g = strchr(g,'\"');

			g++; /*now we have start of galactic word*/

			i = strchr(g,'\"');

			*i = (char)0; /*null out end of	gal word*/
			/*	printf ("G: >%s< / E: >%s<\n",g,e);*/

			ecop = (char*)malloc(strlen(e)+1);
			gcop = (char*)malloc(strlen(g)+1);
			strcpy(ecop,e);
			strcpy(gcop,g);
			voc[wcount][ENG]=ecop;
			voc[wcount][GAL]=gcop;

			wcount++;
		}
	fclose (RD);
	}

	return wcount;
}

/*
 _
| | ___	__ _ _ __ ___
| |/ __/ _` | '__/ __|
| | (_|	(_| | |	 \__ \
|_|\___\__,_|_|	 |___/

display	output in lcars	format

 */

void lcars (char *input,char *output,char *file){

  char iline[3][51];
  char oline[3][51];
  int x,p,l;

  char *pos;

  for (x=0;x<3;x++){
    iline[x][0]	= '\0';
    oline[x][0]	= '\0';
  }

  pos =	input;
  p = 0;
  l = 0;
  for (x=0;((x<strlen(input))&&(x<150));x++){
    iline[l][p++] = *pos++;
    if (p>49) {	    iline[l][p]	= '\0';p = 0; l++;}
  }
  iline[l][p] =	'\0';

  pos =	output;
  p = 0;
  l = 0;
  for (x=0;((x<strlen(output))&&(x<150));x++){
    oline[l][p++] = *pos++;
    if (p>49) {	    oline[l][p]	= '\0';p = 0; l++;}
  }
  oline[l][p] =	'\0';


#ifdef UNIX
system ("clear");
#else
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
  printf ("\n");
#endif
  printf ("  ________________________________________________________________________ \n");
  printf (" / UTA			CASE MODE %-20s		      \\ \n",cases[CASE]);
  printf (" |	      _______________________________________________________________/ \n");
  printf (" |________|\n");
  printf (" |_mode___| %-50s\n",iline[0]);
  printf (" |_%4s___| %-50s\n",modes[mode],iline[1]);
  printf (" |________| %-50s \n",iline[2]);
  printf (" |________| -------------------------------------------------------------\n");
  printf (" |________| %-50s\n",oline[0]);
  printf (" |________| %-50s\n",oline[1]);
  printf (" |________| %-50s\n",oline[2]);
  printf (" |	     |_______________________________________________________________\n");
  printf (" | Commands:	? ! < >	= + *quit *lcars *case FILE: %-12s	      \\ \n",file);
  printf (" \\________________________________________________________________________/\n");
  printf ("\n");
  printf ("\n");
}

/*
	       _ _		_ _	 _
__	___ __(_) |_ ___     __| (_) ___| |_
\ \ /\ / / '__|	| __/ _	\   / _` | |/ __| __|
 \ V  V	/| |  |	| ||  __/  | (_| | | (__| |_
  \_/\_/ |_|  |_|\__\___|___\__,_|_|\___|\__|
		       |_____|

*/
int write_dict(char *filename){
	FILE *WT;
	int x;
	char *ecop,*gcop;
	char FILENM[1000];

	strcpy (FILENM,FILEHOME);
	strcat (FILENM,filename);

	if (WT = fopen(FILENM,"w")){

	for (x=0;x<wcount;x++){
	  fprintf (WT,"\"%s\", \"%s\"\n",voc[x][ENG],voc[x][GAL]);
	}
	fclose (WT);
	return (x);
   }
	return (0);
}



/*

		    __ _
  ___  _ __  _ __  / _|	|
 / _ \|	'_ \| '_ \| |_|	|
| (_) |	|_) | |	| |  _|	|
 \___/|	.__/|_|	|_|_| |_|
      |_|

open file for translation input.

*/
void opnfl (char *filename) {

  FILE *openit;


  if (jcmp ("-",filename)){


    if (openit = fopen(filename,"r")){

	  if (normin !=	stdin) {
		fclose (normin);
		normin = stdin;
	  }
      normin = openit;
    }else{
      fprintf (normout,"error -	INPUT: %s not opened\n",filename);
      normin = stdin;
    }
  }

}


/*
			       _
  ___  _ __  _ __   ___	 _   _|	|_
 / _ \|	'_ \| '_ \ / _ \| | | |	__|
| (_) |	|_) | |	| | (_)	| |_| |	|_
 \___/|	.__/|_|	|_|\___/ \__,_|\__|
      |_|

    open file for output

*/
void opnout (char *filename) {

  FILE *openit;
  char FILENM[1000];

  if (jcmp ("-",filename)){

    if (openit = fopen(filename,"w")){
	  if (normout != stdout) {
	     fclose (normout);
		normout	= stdout;
      }
      normout =	openit;
    }else{
      normout =	stdout;
      fprintf (normout,"error -	OUTPUT:	%s not opened\n",filename);
    }
  }

}



/*
 _			 _
| |_ _ __ __ _ _ __  ___| |_ _ __ ___  __ _ _ __ ___
| __| '__/ _` |	'_ \/ __| __| '__/ _ \/	_` | '_	` _ \
| |_| |	| (_| |	| | \__	\ |_| |	|  __/ (_| | | | | | |
 \__|_|	 \__,_|_| |_|___/\__|_|	 \___|\__,_|_| |_| |_|

translate an input stream to an	output stream

*/

void transtream	(){

  char *word;
  while	(!feof(normin)){
    fgets (input,sizeof(input),normin);
    if (!feof(normin)){
      if (strlen(input)>1){
	word=strtok(input,TOKENS);
	if (word != NULL){
	  fprintf (normout,"%s ",trans(word,mode));
	}
	while (word=strtok(NULL,TOKENS)){
	  fprintf (normout,"%s ",trans(word,mode));
	}/*while tokens*/
      }/*if input*/

    }
    fprintf (normout,"\n");
  }

	if (normin != stdin) {
		fclose (normin);
		normin = stdin;
	}
	if (normout != stdout) {
		fclose (normout);
		normout	= stdout;

	}
}



/*
 _			 _	  _
| |_ _ __ __ _ _ __  ___| |_ _ __(_)_ __   __ _
| __| '__/ _` |	'_ \/ __| __| '__| | '_	\ / _` |
| |_| |	| (_| |	| | \__	\ |_| |	 | | | | | (_| |
 \__|_|	 \__,_|_| |_|___/\__|_|	 |_|_| |_|\__, |
					  |___/
translate an input string to an	output string

*/

void transtring	(char *out, char *in){

  char *word;

  char INP[5000];/* copy for token processing*/

  strcpy (INP,in);

  out[0] = '\0';

  if (strlen(INP)>1){
    word=strtok(INP ,TOKENS);
    if (word !=	NULL){
      strcat (out,trans(word,mode));
      strcat (out," ");
    }
    while (word=strtok(NULL,TOKENS)){
      strcat (out,trans(word,mode));
      strcat (out," ");
    }/*while tokens*/
  }/*if	input*/
}





/*
		 _
 _ __ ___   __ _(_)_ __
| '_ ` _ \ / _`	| | '_ \
| | | |	| | (_|	| | | |	|
|_| |_|	|_|\__,_|_|_| |_|


*/

main (int argc,	char *argv[]) {
	char *word;
	int x;
	normout	= stdout;
	normin = stdin;

	if (argc == 1){

		printf ("reading default dictionary, %s\n",DFLT_DICT);
		printf ("%d words read\n",read_dict(DFLT_DICT));
		printf ("   commandline	input \"uta -e word word2 word3\"\n\ttranslates	FROM english\n\n");
		printf ("   commandline	input \"uta -g word word2 word3\"\n\ttranslates	FROM galactic\n\n");
		printf ("   commandline	input \"uta -f vulcan.lng word ..\"\n\treads vulcan file and translate FROM english\n\n");
		usage();
		while (!done){
			printf ("\n? ");
			gets(input);

			if (feof(stdin)) {
				done = 1;
				continue;
			}

			if (!jcmp (input,"?")){

				usage();

				continue;
			}

			if (*input == '+'){
			char *worda, *wordb, *ecop, *gcop;
			if (strlen(input)>1){
			  worda=strtok(input,TOKENS);
			  wordb=strtok(NULL,TOKENS);
			if (worda && wordb){
			ecop = (char*)malloc(strlen(worda)+1);
			gcop = (char*)malloc(strlen(wordb)+1);
			strcpy(ecop,worda);
			strcpy(gcop,wordb);
			voc[wcount][ENG]=ecop;
			voc[wcount][GAL]=gcop;
			wcount++;
			write_dict("temp.lng");
			printf ("\nadded %s/%s - dictionary now	has %d words\n\n",worda,wordb,wcount);

				}else{
				printf ("\n\toops - + requires a worda/wordb pair\n\n");
				}
			}


				continue;
			}


			if (!jcmp (input,"=")){
				printf ("MODE:");
				if (mode == GAL) {
					printf (" galactic input.\n");
				} else if (mode	== ENG){
					printf (" english input.\n");
				} else {
				  printf (" Automatic (english/galactic) input.\n");
				}
				continue;
			}

			if (!jcmp (input,"*quit")){
				done = 1;
				continue;
			}

			if (!jcmp (input,"*lcars")){
			  if (VERBOSE == 0){
			    VERBOSE = 1;
			    lcars ("","",curfile);
			  } else {
			    VERBOSE = 0;
			  }
			  continue;
			}

			if (!jcmp (input,"*case")){
			  if (CASE == 0){
			    CASE = 1;
				if (VERBOSE ==1){   lcars ("","",curfile);}
				printf (" [case	sensitive now ]	\n");
			  } else {
			    CASE = 0;
				if (VERBOSE ==1){   lcars ("","",curfile);}
				printf (" [not case sensitive now ] \n");
			  }
			  continue;
			}


			if (input[0] ==	'<'){
				printf ("\n\tREADING \"%s\" for	language.",input+1);
				printf (" %d words read\n\n",read_dict(input+1));
				continue;
			}

			if (input[0] ==	'>'){
				printf ("\n\tWriting \"%s\" for	language.",input+1);
				printf (" %d words written\n\n",write_dict(input+1));
				continue;
			}

			if (!jcmp (input,"!")){
				if (mode == GAL) {
					mode =AUTO;
					printf ("now Auto (english/galactic) input.\n");
					continue;
				}
				if (mode == AUTO) {
					mode =ENG;
					printf ("now english input.\n");
					continue;
				}
				if (mode == ENG) {
					mode =GAL;
					printf ("now galactic input.\n");
					continue;
				}
			}

			transtring(OUTP,input);
			if (VERBOSE){
			  lcars	(input,OUTP,curfile);
			}else{
			  printf ("\t>>	%s\n",OUTP);
			}
		}
	} else{

		/*assume command line input*/
		printf ("\n------------\n");
		printf ("reading default dictionary, %s\n",DFLT_DICT);
		printf ("%d words read\n",read_dict(DFLT_DICT));
		for (x=1;x<argc; x++){
			if (!jcmp (argv[x] ,"-f")){
			  x++;	/*go to	next arg, a file name*/
			  printf ("\n--read %d words from %s ----------\n",
			  read_dict(argv[x]),argv[x]);
			} else if (!jcmp (argv[x] ,"-h")){
				usage();
				printf ("command line options:\n");
				printf ("\n");
				printf ("  uta	(-f langfile ) (-o outputfile) (-i inputfile) (-[e|g|a]) text\n");
				printf ("\n");
				printf ("  -f langfile	switch to langfile \n");
				printf ("  -o filename	prints output to filename (- for stdout) (must come before -i)\n");
				printf ("  -i filename	reads input from filename (- for stdin)\n");
				printf ("  -c		no case	sensitivity mode\n");
				printf ("  +c		case sensitivity mode\n");
				printf ("  -e		English	mode\n");
				printf ("  -g		Galactic mode\n");
				printf ("  -a		Auto mode\n");
				printf ("  -q		quiet (not verbose)\n");
				printf ("\n");
				printf ("    text translated according to mode and language file.\n");
				printf ("\n");
				printf ("\n");

				continue;
			} else if (!jcmp (argv[x] ,"-i")){
				x++;	/*go to	next arg, a file name*/
				opnfl(argv[x]);	/*new input stream*/
				transtream();
				continue;
			} else if (!jcmp (argv[x] ,"-o")){
				x++;	/*go to	next arg, a file name*/
				opnout(argv[x]); /*redirect output */
				continue;
			} else if (!jcmp (argv[x] ,"-c")){
			  CASE = 0;
			} else if (!jcmp (argv[x] ,"+c")){
			    CASE = 1;
			} else if (!jcmp (argv[x] ,"-g")){
				mode=GAL;
				printf ("\n------------\n");
			} else if (!jcmp (argv[x] ,"-e")){
				mode=ENG;
				printf ("\n------------\n");
			} else if (!jcmp (argv[x] ,"-a")){
				mode=AUTO;
				printf ("\n------------\n");
			} else if (!jcmp (argv[x] ,"-q")){
				silent = 1; /*suppress commentary*/
			} else {
			  printf ("%s ",trans(argv[x],mode));
			}
		}
		printf ("\n------------\n");
	}

	docommercial();

}
