/*
 * arithmetic3.c
 *
 *  Created on: 27 oct. 2018
 *      Author: Stephane
 */

#include "rsa.h"

void encode(INT v, NB *n, int p10);
void sumINTr(INT v, INT w, int r, INT *msb, INT *lsb);
void multINTbyINT(INT v, INT w, INT *msb, INT *lsb);
void decaleNB(NB *n, int count);

NB atoNBfct(char *str) {
	NB n = newNB();
	int taillePaquet[] = { -1, 2, 4, -1, 9 };
	int p10[] = { -1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
			100000000, 1000000000 };
	int nbChiffres = taillePaquet[sizeof(INT)];
	INT nb = 0;
	p10[0] = p10[nbChiffres];
	for (int i = 1; i <= strlen(str); i++) {
		assert(str[i - 1] >= '0' && str[i - 1] <= '9');
		nb = nb * 10 + val(str[i - 1]);
		if ((i % nbChiffres == 0) || (i == strlen(str))) {
			encode(nb, &n, p10[i % nbChiffres]);
			nb = 0;
		}
	}
	return n;
}

NB sumNBfct(NB a, NB b) {
	NB res = newNB();
	int i, r = 0;
	INT msb, lsb;
	for (i = 0; (i < a.lg) || (i < b.lg); i++) {
		sumINTr(a.nb[i], b.nb[i], r, &msb, &lsb);
		res.nb[i] = lsb;
		r = msb;
	}
	res.lg = i;
	if (r != 0) {
		res.nb[i + 1] = r;
		(res.lg)++;
	}
	return res;
}

NB multNBbyINTfct(NB n, INT a) {
	NB res = newNB();
	INT msb, lsb, r = 0;
	for (int i = 0; i < n.lg; i++) {
		multINTbyINT(n.nb[i], a, &msb, &lsb);
		lsb += r;
		r = msb + (lsb < r);
		res.nb[i] = lsb;
	}
	res.lg = n.lg;
	if (r > 0) {
		res.nb[res.lg] = r;
		(res.lg)++;
	}
	return res;
}

NB multNBbyNBfct(NB a, NB b) {
	NB res = newNB();
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
		resIntermediaire = multNBbyINTfct(*n1, n2->nb[i]);
		decaleNB(&resIntermediaire, i);
		res = sumNBfct(res, resIntermediaire);
	}
	return res;
}
