#ifndef RSA_H_INCLUDED
#define RSA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

/**
 * Type d'entier utilisé dans la structure NB
 */
#define INT uint32_t

/**
 * Taille du tableau d'entier de la structure NB
 */
#define LG_MAX 30

/**
 * Retourne la valeur numérique d'un caractère digital.
 */
#define val(c)  c-'0'

/**
 * Retourne a si a > b, b sinon.
 */
#define max(a,b) (a>b)?a:b

/**
 * Comporte un tableau d’entier servant au stockage d’un nombre en multi précision.
 * Pour faciliter la lecture de ce nombre, la structure comporte également un entier servant au stockage du nombre d’entiers utilisés.
 * Enfin, un entier booléen est utilisé pour conserver le signe du nombre.
 */
typedef struct {
	INT nb[LG_MAX];
	int lg;
	int signe;					// 1 = positif,  -1 = négatif
} NB;

/**
 * Stocke un couple clé publique / clé privée sous la forme de trois NB (n, e, d).
 * La clé publique est alors le couple (n, e) et la clé privée le couple (n, d)
 */
typedef struct {
	NB n;
	NB e;
	NB d;
} Cles;

NB newNB();
void razNB(NB *n);
int lireBitFichier(FILE *f, int *err, int reset);
NB lireBlockFichier(FILE *f, int nbBits, int *err);
void ecrireBlockFichier(FILE *f, NB v, int nbBits, int err, int reset);
int getBitNB(NB n, int i);
int nbBitNB(NB n);
void decaleBitNB(NB *n, int count);
void affiche(char *m, NB n);
void afficheln(char *m, NB n);
void affiche_cles(char *msg, Cles c);
void multNBbyNB(NB a, NB b, NB *res);
void encode(INT v, NB *n, int p10);
void sumINTr(INT v, INT w, int r, INT *msb, INT *lsb);
void substINTr(INT v, INT w, int *r, INT *res);
void multINTbyINT(INT v, INT w, INT *msb, INT *lsb);
void decaleNB(NB *n, int count);
int isGreaterOrEqual(NB a, NB b);
int isEqual(NB a, NB b);
NB INTtoNB(INT i);
NB atoNBfct(char *str);
NB sumNBfct(NB a, NB b);
NB substNBfct(NB a, NB b);
NB multNBbyNBfct(NB a, NB b);
NB resNBbyNBfct(NB a, NB b);
NB expNBbyNBfct(NB a, NB n);
NB expmodNBfct(NB a, NB n, NB m);
NB NBAlea(NB l);
NB NBAleaNB(int l);
NB NBAleaPremier(int l);
NB pgcdNBfct(NB a, NB b);
NB euclideNBfct(NB a, NB b);
NB sumNB(NB a, NB b);
NB substNB(NB a, NB b);
NB quoNBbyNBfct(NB a, NB b);
int isGreater(NB a, NB b);
NB substmultNBfct(NB a, NB b, NB c);
void enregistreCles(Cles c, FILE *publique, FILE *privee);
Cles lireClePublique(FILE *f);
Cles lireClePrivee(FILE *f);
void encryptFile(Cles c, FILE *r, FILE *w);
void decryptFile(Cles c, FILE *r, FILE *w);
Cles genereCles();

#endif // RSA_H_INCLUDED
