#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "musique.h"


int get_octave(char *tonique){
	int res = 0;
	int i;
	for (i=0; i<strlen(tonique); i++){
		if (tonique[i]>='0' && tonique[i]<='9')
			res = tonique[i] - '0';
	}
	return res;
}

char *get_note2(int addtonote, char *tonique, int octave, char *info1){
	if (addtonote == 0)
		return "r";
	int note_tonique = get_tonique_int(tonique);
	int diff = get_diff(tonique);
	int gamme = -1;

	if (tonique[strlen(tonique)-1]=='M'){
		gamme = 1;
	}
	if (tonique[strlen(tonique)-1]=='m'){
		gamme = 0;
	}
	assert (gamme!=-1);
	while (addtonote > 7){
		addtonote = addtonote - 7;
	}
	while (addtonote < -7){
		addtonote = addtonote + 7;
	}
	if (addtonote>0){
        if (addtonote>4){
        }
    }
    if (addtonote<0){
        if (addtonote<-4){
        }
    }
	char *note_brute = get_note3(addtonote, note_tonique, diff, gamme);

	char *note_modif = set_diff(note_brute, info1);
    
    char *octave_char = get_octave_char(octave);

    char *buffer = malloc(1000*sizeof(char));
	sprintf(buffer, "%s%s", note_modif, octave_char);
	return buffer;
}

char *get_octave_char(int octave){
	char *c = malloc(1000* sizeof(char));
	while (octave>0){
		strcat(c, "\'");
		octave--;
	}
	while (octave<0){
		strcat(c, ",");
		octave++;
	}
}

char *set_diff(char* note, char *info1){
	char *c = malloc(100*sizeof(char));
	strcat (c, note);
	if (info1[0]=='\0')
		return note;
	if (is_diese(note)){
		switch(info1[0]){
			case '#' : strcat(c, "is");
			case 'b' : c[strlen(c)-2]='\0';
		}
	}
	else if (is_bemol(note)){
		switch(info1[0]){
			case '#' : c[strlen(c)-2]='\0';
			case 'b' : strcat(c, "es");
		}
	}
	else{
		switch(info1[0]){
			case '#' : strcat(c, "is");
			case 'b' : strcat(c, "es");
		}
	}
	return c;
}

int is_diese(char *note){
	if (strlen(note)>2){
		if (note[strlen(note)-2]=='i' && note[strlen(note)-1]=='s'){
			return 1;
		}
	}
	return 0;
}

int is_bemol(char *note){
	if (strlen(note)>2){
		if (note[strlen(note)-2]=='e' && note[strlen(note)-1]=='s'){
			return 1;
		}
	}
	return 0;
}

char *get_note_bemol(int realaddtonote, int realnote){
	int finalnote = realnote + realaddtonote;
	while (finalnote > 11){
		finalnote -= 12;
	}
	while (finalnote <0){
		finalnote +=12;
	}
	switch (finalnote){
		case 0: return "a";
		case 1: return "bes";
		case 2: return "ces";
		case 3: return "c";
		case 4: return "des";
		case 5: return "d";
		case 6: return "ees";
		case 7: return "fes";
		case 8: return "f";
		case 9: return "ges";
		case 10: return "g";
		case 11: return "aes";
	}
}

char *get_note_diese(int realaddtonote, int realnote){
	int finalnote = realnote + realaddtonote;
	while (finalnote > 11){
		finalnote -= 12;
	}
	while (finalnote <0){
		finalnote +=12;
	}
	switch (finalnote){
		case 0: return "a";
		case 1: return "ais";
		case 2: return "b";
		case 3: return "c";
		case 4: return "cis";
		case 5: return "d";
		case 6: return "dis";
		case 7: return "e";
		case 8: return "f";
		case 9: return "fis";
		case 10: return "g";
		case 11: return "gis";
	}
}
/*
0 La
1 Sib La# -> pas de gamme la#
2 Si Dob -> les 2 gammes
3 Do Si# -> pas de gamme si#
4 Do# Reb -> les 2 gammes
5 Re 
6 Mib Re# -> pas de Re#
7 Mi Fab -> pas de Fab
8 Fa Mi# -> pas de Mi#
9 Fa# Solb -> les 2 gammes
10 Sol
11 Lab Sol# -> pas de Sol#
*/
char *get_note3(int addtonote, int note_tonique, int diff, int gamme){
	if (addtonote < 0){
		addtonote = 9+addtonote;
	}
	if (addtonote == 8){
		addtonote = 1;
	}
	int realaddtonote;
	switch (addtonote){
		case 1: realaddtonote = 0; break;
		case 2: realaddtonote = 2; break; 
		case 3: realaddtonote = 3+gamme; break;
		case 4: realaddtonote = 5; break;
		case 5: realaddtonote = 7; break;
		case 6: realaddtonote = 8+gamme; break;
		case 7: realaddtonote = 10+gamme; break;
	}
	switch (note_tonique){
		case 0: //Do
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 2);
				case 0:
					return get_note_bemol(realaddtonote, 3);
				case 1: //dièse
					return get_note_diese(realaddtonote, 4);
			}
		case 1: //Re
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 4);
				case 0:
					return get_note_bemol(realaddtonote, 5);
				case 1: //dièse
					return get_note_bemol(realaddtonote, 6);
			}
		case 2: //Mi
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 6);
				case 0:
					return get_note_diese(realaddtonote, 7);
				case 1: //dièse
					return get_note_bemol(realaddtonote, 8);
			}
		case 3: //Fa
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 7);
				case 0:
					return get_note_bemol(realaddtonote, 8);
				case 1: //dièse
					return get_note_diese(realaddtonote, 9);
			}
		case 4: //Sol
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 9);
				case 0:
					return get_note_diese(realaddtonote, 10);
				case 1: //dièse
					return get_note_bemol(realaddtonote, 11);
			}
		case 5: //La
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 11);
				case 0:
					return get_note_diese(realaddtonote, 0);
				case 1: //dièse
					return get_note_bemol(realaddtonote, 1);
			}
	case 6: //Si
			switch (diff){
				case -1: //bemol
					return get_note_bemol(realaddtonote, 1);
				case 0:
					return get_note_diese(realaddtonote, 2);
				case 1: //dièse
					return get_note_bemol(realaddtonote, 3);
				}
		}
}

int get_tonique_int(char *tonique){
	switch(tonique[0]){
		case 'D' : return 0;
		case 'R' : return 1;
		case 'M' : return 2;
		case 'F' : return 3;
		case 'S' :
			if (tonique[1]=='o')
				return 4;
			else
				return 6;
		case 'L' : return 5;
		default : assert(0);
	}
}

int get_diff(char *tonique){
	int i;
	for (i=0; i<strlen(tonique); i++){
		if (tonique[i]=='b')
			return -1;
		if (tonique[i]=='#')
			return 1;
	}
	return 0;
}