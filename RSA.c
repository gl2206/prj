/*
 * RSA.c
 */
#include "RSA.h"

void printHelp() {
	printf(" Usage: RSA -deg \n");
	printf(
			" -d fichier.txt cle.key decrypte le fichier fichier.txt en utilisant le fichier cle.key\n");
	printf(
			" -e fichier.txt cle.key crypte le fichier fichier.txt en utilisant le fichier cle.key\n");
	printf(" -g genere les cles et les sauve dans public.key et private.key\n\n");
}

int main(int argc, char** argv) {
//	srand((unsigned) time(NULL));
	switch (argc) {
//	case 1 : printHelp(); break;
	case 2:
		if (!strcmp(argv[1], "-g")) {
			Cles c = genereCles();
			FILE *public = fopen("public.key", "wb"), *private = fopen(
					"private.key", "wb");
			assert(public != NULL && private != NULL);
			enregistreCles(c, public, private);
			fclose(public);
			fclose(private);
		} else
			printHelp();
		break;
//	case 3 : printf(""); printHelp(); break;
	case 4:
		if (!strcmp(argv[1], "-g")) {
			Cles c = genereCles();
			FILE *public = fopen(argv[2], "wb"), *private = fopen(argv[3],
					"wb");
			assert(public != NULL && private != NULL);
			enregistreCles(c, public, private);
			fclose(public);
			fclose(private);
		} else if (!strcmp(argv[1], "-e")) {
			char *str = (char*) malloc(strlen(argv[2]) + 7);
			strcpy(str, argv[2]);
			strcat(str, ".crypt");
			FILE *f = fopen(argv[2], "rb"), *key = fopen(argv[3], "rb"),
					*crypt = fopen(str, "wb");
			assert(f != NULL && key != NULL && crypt != NULL);
			Cles c = lireClePublique(key);
			encryptFile(c, f, crypt);
			fclose(f);
			fclose(key);
			fclose(crypt);
			free(str);
		} else if (!strcmp(argv[1], "-d")) {
			char *str;
			FILE *f = fopen(argv[2], "rb");
			if (strstr(argv[2], ".crypt") != NULL) {
				str = (char*) malloc(strlen(argv[2]) - 6);
				char *tmp = strstr(argv[2], ".crypt");
				*tmp = '\0';
				strcpy(str, argv[2]);
			} else {
				str = (char*) malloc(strlen(argv[2]) + 9);
				strcpy(str, argv[2]);
				strcat(str, ".decrypt");
			}
			FILE *key = fopen(argv[3], "rb"), *decrypt = fopen(str, "wb");
			assert(f != NULL && key != NULL && decrypt != NULL);
			Cles c = lireClePrivee(key);
			decryptFile(c, f, decrypt);
			fclose(f);
			fclose(key);
			fclose(decrypt);
			free(str);
		} else
			printHelp();
		break;
	case 5:
		if (!strcmp(argv[1], "-e")) {
			FILE *f = fopen(argv[2], "rb"), *key = fopen(argv[4], "rb"),
					*crypt = fopen(argv[3], "wb");
			assert(f != NULL && key != NULL && crypt != NULL);
			Cles c = lireClePublique(key);
			encryptFile(c, f, crypt);
			fclose(f);
			fclose(key);
			fclose(crypt);
		} else if (!strcmp(argv[1], "-d")) {
			FILE *f = fopen(argv[2], "rb"), *key = fopen(argv[4], "rb"),
					*decrypt = fopen(argv[3], "wb");
			assert(f != NULL && key != NULL && decrypt != NULL);
			Cles c = lireClePrivee(key);
			decryptFile(c, f, decrypt);
			fclose(f);
			fclose(key);
			fclose(decrypt);
		} else
			printHelp();
		break;
	default:
		printHelp();
	}
	return 0;
}

