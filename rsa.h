#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

#define INT uint8_t
#define LG_MAX 20

#define val(c)  c-'0'
#define max(a,b) (a>b)?a:b

typedef struct {
	INT nb[LG_MAX];
	int lg;
} NB;

NB newNB();
void razNB(NB *n);
void affiche(NB n);
void atoNB(char * str, NB* n);
void sumNB(NB a, NB b, NB *res);
void multNBbyNB(NB a, NB b, NB *res);

#endif // RSA_H_INCLUDED
