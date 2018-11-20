#include "rsa.h"

/**
 * NewNB: "construit" un nouveau NB initialise a 0.
 */
NB newNB() {
	NB n = { .lg = 0 };
	int i;
	for (i = 0; i < LG_MAX; i++) {
		n.nb[i] = 0;
	}
	return n;
}

/**
 * razNB: reinitialise un NB deja construit a zero.
 */
void razNB(NB *n) {
	int i;
	n->lg = 0;
	for (i = 0; i < LG_MAX; i++) {
		n->nb[i] = 0;
	}
}

/**
 * affiche un NB sur l'écran en hexadécimal
 *
 */
void affiche(NB n) {
	for (int i = n.lg - 1; i >= 0; i--)
		printf("%02x", n.nb[i]);
	printf(" %d EOF\n", n.lg);

}

/**
 * mult INT x INT, retour ds msb et lsb
 */
void multINTbyINT(INT v, INT w, INT *msb, INT *lsb) {
	int taille = sizeof(INT);
	uint64_t res = (uint64_t) v * w, deuxP32 = 1;
	deuxP32 = deuxP32 << (taille * 8);
	//*lsb=(INT) (res%(long)pow(2,taille*8));
	*lsb = (INT) (res % deuxP32);
	*msb = (INT) (res >> taille * 8);
	//printf(" %d %u %d %u %u \n", taille, v, w, *msb, *lsb);
}

/**
 * multiplication d'un NB par un INT
 *
 */
void multNBbyINT(NB n, INT a, NB *res) {
	INT msb, lsb, r = 0;
	for (int i = 0; i < n.lg; i++) {
		multINTbyINT(n.nb[i], a, &msb, &lsb);
		lsb += r;
		r = msb + (lsb < r);
		res->nb[i] = lsb;
	}
	res->lg = n.lg;
	if (r > 0) {
		res->nb[res->lg] = r;
		(res->lg)++;
	}
}

/**
 * decale un NB d'un certain nombre de INT
 */
void decaleNB(NB *n, int count) {
	n->lg += count;
	for (int i = n->lg - 1; i >= 0; i--) {
		if (i > count - 1) { // recopie la case
			n->nb[i] = n->nb[i - count];
		} else { // injecte des zeros
			n->nb[i] = 0;
		}
	}
}
/**
 * multiplication de 2 NB
 *
 */
void multNBbyNB(NB a, NB b, NB *res) {
	NB *n1, *n2;
	int i;
	// positionne dans n1 le NB le plus long
	if (a.lg > b.lg) {
		n1 = &a;
		n2 = &b;
	} else {
		n1 = &b;
		n2 = &a;
	}
	for (i = 0; i < n2->lg; i++) {
		NB resIntermediaire = newNB();
		multNBbyINT(*n1, n2->nb[i], &resIntermediaire);
		decaleNB(&resIntermediaire, i);
		sumNB(*res, resIntermediaire, res);
	}
}

/**
 * conversion INT -> NB
 *
 */
NB INT2NB(INT n) {
	NB n_v = newNB();
	n_v.lg = 1;
	n_v.nb[0] = n;
	return n_v;
}

/**
 *  n <- n*p10 + v
 */
void encode(INT v, NB *n, int p10) {
	//printf(" %d %d\n",v,p10);

	INT msb, lsb;
	for (int i = 0; i < n->lg; i++) {
		multINTbyINT(n->nb[i], p10, &msb, &lsb);
		lsb += v;
		v = msb + (lsb < v);
		n->nb[i] = lsb;
	}
	if (v > 0) {
		n->nb[n->lg] = v;
		(n->lg)++;
	}
//	NB p=newNB();
//	multNBbyINT(*n, (INT)p10, &p);
//	sumNB(*p, INT2NB(v), n);
}

/**
 * encode un char * en NB
 */
void atoNB(char * str, NB* n) {
	int taillePaquet[] = { -1, 2, 4, -1, 9 };
	int p10[] = { -1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
			100000000, 1000000000 };
	int nbChiffres = taillePaquet[sizeof(INT)];
	INT nb = 0;
	p10[0] = p10[nbChiffres];
	for (int i = 1; i <= strlen(str); i++) {
		assert(str[i - 1] >= '0' && str[i - 1] <= '9');
		//printf("%c\n",str[i-1]);
		nb = nb * 10 + val(str[i - 1]);
		if ((i % nbChiffres == 0) || (i == strlen(str))) {
			encode(nb, n, p10[i % nbChiffres]);
			nb = 0;
		}
	}
}

/**
 * Somme entre deux INT + retenue
 */
void sumINTr(INT v, INT w, int r, INT *msb, INT *lsb) {
	int taille = sizeof(INT);
	uint64_t res = (uint64_t) v + w + r, deuxP32 = 1;
	deuxP32 = deuxP32 << (taille * 8);
	*lsb = (INT) (res % deuxP32);
	*msb = (INT) (res >> taille * 8);
	//if (DEBUG) printf(" %d %u %d %u %u \n", taille, v, w, *msb, *lsb);
}

/**
 * Somme entre deux INT
 */
void sumINT(INT v, INT w, INT *msb, INT *lsb) {
	sumINTr(v, w, 0, msb, lsb);
}

/**
 * Somme entre deux NB
 */
void sumNB(NB a, NB b, NB *res) {
	int i, r = 0;
	INT msb, lsb;
	for (i = 0; (i < a.lg) || (i < b.lg); i++) {
		sumINTr(a.nb[i], b.nb[i], r, &msb, &lsb);
		res->nb[i] = lsb;
		r = msb;
	}
	res->lg = i;
	if (r != 0) {
		res->nb[i + 1] = r;
		(res->lg)++;
	}
}
