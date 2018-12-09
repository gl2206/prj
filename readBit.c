/*
 ============================================================================
 Name        : readBit.c
 Author      : gl
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lireBitFichier(FILE *f, int *err) {
	static char c;
	static int numBit;
	if (numBit == 0)
		c = fgetc(f);
	if (feof(f))
		*err = 1;
	else
		*err = 0;
	int b = c & (1 << (7 - numBit));
	if (b != 0)
		b = 1;
	if (numBit < 7)
		numBit++;
	else {
		numBit = 0;
	}
	//printf("lireBit: index=%d, numBits=%d --> %d\n",index,numBit,b);
	return b;
}

long lireBlockFichier(FILE *f, int nbBits, int *err) {
	long result = 0;
	int i, b;
	for (i = 0; i < nbBits; i++) {
		b = lireBitFichier(f, err);
		if (*err == 1)
			break;
//			printf("%x ",result);
		result = result << 1;
		result += b;
//			printf("%d -> %x\n",b,result);
	}
	if (*err == 1)
		result = result << (nbBits - i);
	return result;
}

int ecrireBlockFichier(FILE *f, long v, int nbBits) {
	static int numBit;
	static char c;
	int i, b;
	for (i = 0; i < nbBits; i++) {
		b = v & (1 << (nbBits - i - 1));
		if (b != 0)
			b = 1;
		c <<= 1;
		c += b;
		if (numBit < 7)
			numBit++;
		else {
			fputc(c, f);
			numBit = 0;
		}
	}
	return 0;
}

int lireBitTableau(char *t, int lg, int *err) {
	static int index;
	static int numBit;
	if (index >= lg)
		*err = 1;
	else
		*err = 0;
	int b = t[index] & (1 << (7 - numBit));
	if (b != 0)
		b = 1;
	if (numBit < 7)
		numBit++;
	else {
		index++;
		numBit = 0;
	}
	//printf("lireBit: index=%d, numBits=%d --> %d\n",index,numBit,b);
	return b;
}

long lireBlockTableau(char *t, int nbBits, int *err) {
	long result = 0;
	int i, b, lg = strlen(t);
	for (i = 0; i < nbBits; i++) {
		b = lireBitTableau(t, lg, err);
		if (*err == 1)
			break;
//			printf("%x ",result);
		result = result << 1;
		result += b;
//			printf("%d -> %x\n",b,result);
	}
	if (*err == 1)
		result = result << (nbBits - i);
	return result;
}

int ecrireBlockTableau(char *t, long v, int nbBits) {
	static int index;
	static int numBit;
	int i, b;
	for (i = 0; i < nbBits; i++) {
		b = v & (1 << (nbBits - i - 1));
		if (b != 0)
			b = 1;
		t[index] <<= 1;
		t[index] += b;
		if (numBit < 7)
			numBit++;
		else {
			index++;
			numBit = 0;
		}
	}
	return 0;
}

#define N 3

int main(void) {
	int e = 0;
	//puts("Hello World!!vnvnvn!"); /* prints Hello World!!! */
//	char t[] = "palindromes Hello World!! Hello World !!Hello World!!";
	//char t[] = "abc";
	//char res[500];
	FILE *f = fopen("essai.txt", "r+b");
	FILE *res = fopen("res.txt", "w+b");
	if (f == NULL || res == NULL) {
		printf("erreur\n");
		exit(-1);
	}
	while (1) {
//		long r = lireBlockTableau(t, N, &e);
		long r = lireBlockFichier(f, N, &e);
		//printf("%x+%d ", r, e);
		ecrireBlockFichier(res, r, N);
		if (e == 1)
			break;
	}
/*	for (int i = 0; i < strlen(t); i++) {
		printf("%c", res[i]);
	}
	printf("\n");*/
	fclose(f);
	fseek(res, 0, SEEK_SET);
	while (!feof(res)) putchar(fgetc(res));
	fclose(res);
	return EXIT_SUCCESS;
}
