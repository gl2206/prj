/*
 * RSA_Utilitaires.c
 */
#include "RSA.h"

/**
 * \fct NB newNB()
 * @return un NB initialis� � 0.
 *
 * Construit et retourne un NB initialis� � 0.
 */
NB newNB() {
	NB n = { .lg = 0, .signe = 1 };
	int i;
	for (i = 0; i < LG_MAX; i++) {
		n.nb[i] = 0;
	}
	return n;
}

/**
 * \fct void razNB(NB *n)
 * @param n de type NB* (prend la valeur 0)
 *
 * R�initialise un NB � 0.
 */
void razNB(NB *n) {
	int i;
	n->lg = 0;
	for (i = 0; i < LG_MAX; i++) {
		n->nb[i] = 0;
	}
}

/**
 * \fct void affiche(char *msg, NB n)
 * @param msg de type char*
 * @param n de type NB
 *
 * Affiche sur la console le NB n en hexad�cimal suivi de sa longueur(en entier et en nombre de bits) et pr�c�d� d'un message �ventuel msg.
 *
 * Attention : ne saute pas � la ligne apr�s l'affichage.
 */
void affiche(char *msg, NB n) {
	if (n.signe == -1)
		printf("%s -0x", msg);
	else
		printf("%s 0x", msg);
	for (int i = n.lg - 1; i >= 0; i--)
		printf("%08x", n.nb[i]);
	printf(" (%d) %d bits", n.lg, nbBitNB(n));

}

/**
 * \fct void afficheln(char *msg, NB n)
 * @param msg de type char*
 * @param n de type NB
 *
 * Affiche sur la console le NB n en hexad�cimal suivi de sa longueur (en entier et en nombre de bits) et pr�c�d� d'un message �ventuel msg.
 * Apr�s avoir affich�, saute � la ligne.
 */
void afficheln(char *msg, NB n) {
	affiche(msg, n);
	printf("\n");
}

/**
 * \fct void affiche_cles(char *msg, Cles c)
 * @param msg de type char *
 * @param c de type Cles
 *
 * Affiche sur la console les trois NB de la structure Cles c pr�c�d� d'un message �ventuel msg.
 */
void affiche_cles(char *msg, Cles c) {
	printf("%s", msg);
	afficheln("n = ", c.n);
	afficheln("e = ", c.e);
	afficheln("d = ", c.d);
}

/**
 * \fct void encode(INT v, NB *n, int p10)
 * @param v le type INT (le nombre � encoder dans n)
 * @param n de type NB* (prend la valeur n*p10 + v)
 * @param p10 de type int (la puissance de 10 n�cessaire pour "faire de la place" � v)
 *
 * Multiplie le NB stock� par la puissance de 10 n�cessaire puis rajoute v � la droite du NB.
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
}


/**
 * \fct NB atoNBfct(char *str)
 * @param str de type char*
 * @return un NB de valeur str
 *
 * Convertit la cha�ne de caract�re str en un NB de m�me valeur (similaire � atoi pour les int).
 */
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

/**
 *
 * \fct NB INTtoNB(INT i)
 * @param i de type INT
 * @return un NB de valeur i
 *
 * Convertit un INT en un NB.
 *
 */
NB INTtoNB(INT i) {
	NB n = newNB();
	if (i)
		n.lg = 1;
	else
		n.lg = 0;
	n.nb[0] = i;
	return n;
}

