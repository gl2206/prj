/*
 * RSA_Arithmetique.c
 */
#include "RSA.h"

/**
 * \fct int getBitNB(NB n, int i)
 * @param n de type NB
 * @param i de type int
 * @return la valeur du bit numéro i de la valeur de n(0 ou 1)
 *
 * Retourne la valeur du bit numéro i de la valeur de n.
 */
int getBitNB(NB n, int i) {
	if (i < 0 || i > (8 * sizeof(INT)) * n.lg)
		return 0;
	int numOctet = i / (8 * sizeof(INT));
	int numBit = i % (8 * sizeof(INT));
	if (n.nb[numOctet] & (1 << numBit))
		return 1;
	else
		return 0;
}

/**
 * \fct void setBitNB(NB *n, int i, int b)
 * @param n de type NB
 * @param i de type int
 * @param b de type int
 *
 * Positionne la valeur du bit numéro i de n à la valeur b.
 */
void setBitNB(NB *n, int i, int b) {
	if (i < 0)
		return;
	int numOctet = i / (8 * sizeof(INT));
	int numBit = i % (8 * sizeof(INT));
	if (b) {
		n->nb[numOctet] = n->nb[numOctet] | (1 << numBit);
		if (numOctet > n->lg - 1)
			n->lg = numOctet + 1;
	} else {
		n->nb[numOctet] = n->nb[numOctet] & ~(1 << numBit);
		while (n->nb[n->lg - 1] == 0 && n->lg > 0)
			(n->lg)--;
	}
}

/**
 * \fct int nbBitNB(NB n)
 * @param n de type NB
 * @return le nombre de bits de n
 *
 * Retourne le nombre de bits utilisés pour stocker n.
 */
int nbBitNB(NB n) {
	if (n.lg == 0)
		return 0;
	int nbBit = (8 * sizeof(INT)) * n.lg;
	while (!getBitNB(n, nbBit - 1)) {
		nbBit--;
	}
	return nbBit;
}

/**
 * \fct void decaleBitNB(NB *n, int count)
 * @param n de type NB
 * @param count de type int
 *
 * Décale un NB de count bits vers la gauche.
 */
void decaleBitNB(NB *n, int count) {
	if (nbBitNB(*n) == 0)
		return;
	int nb = nbBitNB(*n) + count;
	while (nb > (8 * sizeof(INT)) * n->lg)
		n->lg++;
	for (int i = nb - 1; i >= 0; i--) {
		if (i > count - 1) {
			setBitNB(n, i, getBitNB(*n, i - count));
		} else {
			setBitNB(n, i, 0);
		}
	}
}

/**
 * \fct int isGreaterOrEqual(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return 1 si abs(a) est plus grand ou égal à abs(b), 0 sinon
 *
 * Retourne 1 si abs(a) est plus grand ou égal à abs(b), 0 sinon
 *
 * Attention: comparaison en valeur absolue uniquement.
 */
int isGreaterOrEqual(NB a, NB b) {
	int i;
	if (a.lg < b.lg)
		return 0;
	if (a.lg > b.lg)
		return 1;
	for (i = a.lg; i >= 0; i--) {
		if (a.nb[i] != b.nb[i]) {
			if (a.nb[i] < b.nb[i])
				return 0;
			else
				return 1;
		}
	}
	return 1;
}

/**
 * \fct int isGreaterfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return 1 si abs(a) est strictement plus grand que abs(b), 0 sinon
 *
 * Retourne 1 si abs(a) est strictement plus grand que abs(b), 0 sinon
 *
 * Attention: comparaison en valeur absolue uniquement.
 */
int isGreaterfct(NB a, NB b) {
	int i;
	if (a.lg < b.lg)
		return 0;
	if (a.lg > b.lg)
		return 1;
	for (i = a.lg; i >= 0; i--) {
		if (a.nb[i] != b.nb[i]) {
			if (a.nb[i] < b.nb[i])
				return 0;
			else
				return 1;
		}
	}
	return 0;
}

/**
 * \fct int isGreater(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return 1 si a est strictement plus grand que b, 0 sinon
 *
 * Retourne 1 si a est strictement plus grand que b, 0 sinon, en TENANT compte du signe de a et b.
 *
 */
int isGreater(NB a, NB b) {
	if (a.signe == 1 && b.signe == -1)
		return 1;
	if (a.signe == -1 && b.signe == 1)
		return 0;
	int r = isGreaterfct(a, b);
	if (a.signe == 1)
		return r;
	else
		return 1 - r;
}

/**
 *
 * \fct int isEqual(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return 1 si abs(a) est égal à abs(b), 0 sinon
 *
 * Retourne 1 si abs(a) est égal à abs(b), 0 sinon.
 *
 * Attention: comparaison en valeur absolue uniquement.
 */
int isEqual(NB a, NB b) {
	int i;
	if (a.lg != b.lg)
		return 0;
	for (i = a.lg; i >= 0; i--) {
		if (a.nb[i] != b.nb[i]) {
			return 0;
		}
	}
	return 1;
}

/**
 * \fct NB sumNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur abs(a) + abs(b)
 *
 * Retourne la somme de abs(a) et abs(b).
 *
 * Attention : ne tient pas compte du signe de a et b.
 */
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

/**
 * \fct NB substNBfct(NB x, NB y)
 * @param x de type NB
 * @param y de type NB
 * @return un NB de valeur x - y
 *
 * Retourne la différence de x avec y. Si x < y, alors on calcule y - x et le signe du résultat est négatif.
 */
NB substNBfct(NB x, NB y) {
	NB res = newNB(), a, b;
	if (isGreaterfct(x, y)) {
		a = x;
		b = y;
		res.signe = 1;
	} else {
		a = y;
		b = x;
		res.signe = -1;
	}
	int i, r = 0;
	res.lg = a.lg;
	for (i = 0; i < a.lg; i++)
		substINTr(a.nb[i], b.nb[i], &r, &res.nb[i]);
	while (res.nb[res.lg - 1] == 0 && res.lg > 0)
		(res.lg)--;
	return res;
}

/**
 * \fct NB sumNB(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur a + b
 *
 * Retourne la somme de a et b en TENANT compte du signe de a et b.
 */
NB sumNB(NB a, NB b) {
	NB res;
	if (a.signe == b.signe) {
		res = sumNBfct(a, b);
		res.signe = a.signe;
		return res;
	} else {
		if (a.signe == 1)
			return substNBfct(a, b);
		else
			return substNBfct(b, a);
	}
}
/**
 * \fct NB substNB(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur a - b
 *
 * Retourne la différence de a avec b en TENANT compte du signe de a et b.
 */
NB substNB(NB a, NB b) {
	NB res;
	if (a.signe == b.signe) {
		if (a.signe == 1)
			return substNBfct(a, b);
		else {
			res = substNBfct(a, b);
			res.signe = -res.signe;
			return res;
		}
	} else {
		if (a.signe == 1)
			return sumNBfct(a, b);
		else {
			res = sumNBfct(a, b);
			res.signe = -res.signe;
			return res;
		}
	}
}

/**
 * \fct NB multNBbyINTfct(NB n, INT a)
 * @param n de type NB
 * @param a de type INT
 * @return un NB de valeur n * a
 *
 * Retourne la multiplication du NB n par l'INT a.
 */
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

/**
 * \fct NB multNBbyNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur a * b
 *
 * Retourne la multiplication du NB a par le NB b en TENANT compte du signe.
 */
NB multNBbyNBfct(NB a, NB b) {
	NB res = newNB();
	NB *n1, *n2;
	int i;
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
	res.signe = a.signe * b.signe;
	return res;
}

/**
 * \fct NB substmultNBfct(NB a, NB b, NB c)
 * @param a de type NB
 * @param b de type NB
 * @param c de type NB
 * @return un NB de valeur a - b * c
 *
 * Retourne la différence entre a et le produit de b par c.
 */
NB substmultNBfct(NB a, NB b, NB c) {
	return substNB(a, multNBbyNBfct(b, c));
}

/**
 * \fct void divNBbyNBfct(NB a, NB b, NB *q, NB *r)
 * @param a de type NB
 * @param b de type NB
 * @param q de type NB* (prend la valeur de a / b)
 * @param r de type NB* (prend la valeur de a % b)
 *
 * Retourne dans q et r respectivement le quotient et le reste de la division euclidienne de a par b.
 */
void divNBbyNBfct(NB a, NB b, NB *q, NB *r) {
	assert(isGreater(b, INTtoNB(0)));
	int i;
	razNB(q);
	razNB(r);
	for (i = nbBitNB(a) - 1; i >= 0; i--) {
		decaleBitNB(r, 1);
		if (getBitNB(a, i))
			setBitNB(r, 0, 1);
		if (isGreaterOrEqual(*r, b)) {
			setBitNB(q, i, 1);
			*r = (substNBfct(*r, b));
		}
	}
	q->signe = a.signe * b.signe;
	r->signe = a.signe;
}

/**
 * \fct NB quoNBbyNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur a / b
 *
 * Retourne le quotient de la division euclidienne de a par b.
 */
NB quoNBbyNBfct(NB a, NB b) {
	NB q = newNB();
	NB r = newNB();
	divNBbyNBfct(a, b, &q, &r);
	return q;
}

/**
 * \fct NB resNBbyNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur a % b
 *
 * Retourne le reste de la division euclidienne de a par b.
 */
NB resNBbyNBfct(NB a, NB b) {
	NB q = newNB();
	NB r = newNB();
	divNBbyNBfct(a, b, &q, &r);
	return r;
}

/**
 * \fct NB expmodNBfct(NB a, NB e, NB m)
 * @param a de type NB
 * @param e de type NB
 * @param m de type NB
 * @return un NB de valeur (a^e) % m
 *
 * Retourne le modulo par m de a à la puissance e
 */
NB expmodNBfct(NB a, NB e, NB m) {
	NB res = INTtoNB(1);
	while (isGreater(e, INTtoNB(0))) {
		if (getBitNB(e, 0))
			res = resNBbyNBfct(multNBbyNBfct(res, a), m);
		e = quoNBbyNBfct(e, INTtoNB(2));
		a = resNBbyNBfct(multNBbyNBfct(a, a), m);
	}
	return res;
}

/**
 * \fct NB NBAlea(NB l)
 * @param l de type NB
 * @return un NB aleatoire
 *
 * Retourne un nombre aléatoire entre 0 et
 */
NB NBAlea(NB l) {
	int i;
	NB a = newNB();
	a.lg = l.lg;
	do {
		for (i = 0; i < l.lg; i++) {
			a.nb[i] = (INT) ((rand() << 16) + rand());
		}
	} while (isGreaterOrEqual(a, l));
	while (a.nb[a.lg - 1] == 0 && a.lg > 0)
		(a.lg)--;
	return a;
}

NB NBAleaNB(int l) {
	int i;
	NB a = newNB();
	a.lg = l;
	for (i = 0; i < l; i++) {
		a.nb[i] = (INT) ((rand() << 16) + rand());
	}
	while (a.nb[a.lg - 1] == 0 && a.lg > 0)
		(a.lg)--;
	return a;
}

/**
 * \fct void sumINTr(INT v, INT w, int r, INT *msb, INT *lsb)
 * @param v de type INT
 * @param w de type INT
 * @param r de type int
 * @param msb de type INT* (prend la valeur des bits de poids forts de v + w)
 * @param lsb de type INT* (prend la valeur des bits de poids faibles de v + w)
 *
 * Somme les deux NB v et w ainsi que la retenue r, et range le résultat dans les deux NB msb (bits de poids forts) et lsb (bits de poids faibles).
 */
void sumINTr(INT v, INT w, int r, INT *msb, INT *lsb) {
	int taille = sizeof(INT);
	uint64_t res = (uint64_t) v + w + r, deuxP32 = 1;
	deuxP32 = deuxP32 << (taille * 8);
	*lsb = (INT) (res % deuxP32);
	*msb = (INT) (res >> taille * 8);
}

/**
 * \fct void substINTr(INT v, INT w, int *r, INT *res)
 * @param v de type INT
 * @param w de type INT
 * @param r de type int* (prend la valeur de la retenue de v - w)
 * @param res de type INT* (prend la valeur de v - w)
 *
 * Soustrait au NB v le NB w et la retenue r, range la retenue éventuelle dans r, et range le résultat dans res.
 */
void substINTr(INT v, INT w, int *r, INT *res) {
	if (v >= (w + *r)) {
		*res = v - w - *r;
		*r = 0;
	} else {
		*res = v - (w + *r);
		*r = 1;
	}
}

/**
 * \fct void multINTbyINT(INT v, INT w, INT *msb, INT *lsb)
 * @param v de type INT
 * @param w de type INT
 * @param msb de type INT (prend la valeur des bits de poids forts de v * w)
 * @param lsb de type INT (prend la valeur des bits de poids faibles de v * w)
 *
 * Multiplie les deux NB v et w, et range le résultat dans les deux NB msb (bits de poids forts) et lsb (bits de poids faibles).
 */
void multINTbyINT(INT v, INT w, INT *msb, INT *lsb) {
	int taille = sizeof(INT);
	uint64_t res = (uint64_t) v * w, deuxP32 = 1;
	deuxP32 = deuxP32 << (taille * 8);
	*lsb = (INT) (res % deuxP32);
	*msb = (INT) (res >> taille * 8);
}

/**
 * \fct void decaleNB(NB *n, int count)
 * @param n de type NB*
 * @param count de type int
 *
 * Décale le NB n de count INT vers la gauche.
 */
void decaleNB(NB *n, int count) {
	n->lg += count;
	for (int i = n->lg - 1; i >= 0; i--) {
		if (i > count - 1) {
			n->nb[i] = n->nb[i - count];
		} else {
			n->nb[i] = 0;
		}
	}
}

