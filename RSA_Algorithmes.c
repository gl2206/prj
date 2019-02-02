/*
 * RSA_Algorithmes.c
 */
#include "RSA.h"

/**
 * \fct int temoinMillerNBfct(NB a, NB n)
 * @param a de type NB
 * @param n de type NB
 * @return 0 si a est un témoin de Miller pour n, 1 sinon
 *
 * Teste si a est un témoin de Miller pour n et retourne 1 si oui, 0 sinon.
 */
int temoinMillerNBfct(NB a, NB n) {
	NB s = INTtoNB(0), d = substNBfct(n, INTtoNB(1)), x, temp = substNBfct(n,
			INTtoNB(1));

	//printf("Calcul de s et d\n");
	while (!getBitNB(d, 0)) {
		d = quoNBbyNBfct(d, INTtoNB(2));
		s = sumNBfct(s, INTtoNB(1));
	}

	//printf("Calcul de x\n");
	x = expmodNBfct(a, d, n);

	//printf("Test initial\n");
	if (isEqual(x, INTtoNB(1)) || isEqual(x, temp))
		return 0;

	//printf("Entree boucle\n");
	while (isGreater(s, INTtoNB(1))) {
		x = expmodNBfct(x, INTtoNB(2), n);
		if (isEqual(x, temp))
			return 0;
		s = substNBfct(s, INTtoNB(1));
	}
	return 1;
}

/**
 * \fct int millerRabinNBfct(NB n, int k)
 * @param n de type NB
 * @param k de type int
 * @return 1 si n est probablement premier, 0 s'il n'est pas premier
 *
 * Teste k nombres tirés aléatoirement et retourne 1 si aucun de ces nombres n'est un témoin de Miller pour n, 0 sinon.
 */
int millerRabinNBfct(NB n, int k) {
	int i;
	NB a, temp = substNBfct(n, INTtoNB(4));
	for (i = 0; i < k; i++) {
		a = NBAlea(temp);
		a = sumNBfct(a, INTtoNB(2));
		//affiche("a = ", a);
		if (temoinMillerNBfct(a, n))
			return 0;
	}
	return 1;
}

/**
 * \fct NB NBAleaPremier(NB l)
 * @param l de type NB
 * @return un NB probablement premier
 *
 * Retourne un nombre probablement premier testé avec l'algorithme de Miller-Rabin.
 */
NB NBAleaPremier(int l) {
	NB a;
	do {
		do {
			a = NBAleaNB(l);
		} while (getBitNB(a, 0));
		a = sumNBfct(a, INTtoNB(3));
		//afficheln("a = ", a);
	} while (!millerRabinNBfct(a, 25));
	return a;
}
/**
 * \fct NB pgcdNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur PGCD(a, b)
 *
 * Retourne la valeur du PGCD de a et b.
 *
 * Attention : il faut a > b.
 */
NB pgcdNBfct(NB a, NB b) {
	assert(isGreater(a, b));
	NB r = resNBbyNBfct(a, b);
	while (!isEqual(r, INTtoNB(0))) {
		a = b;
		b = r;
		r = resNBbyNBfct(a, b);
	}
	return b;
}

/**
 * \fct NB euclideNBfct(NB a, NB b)
 * @param a de type NB
 * @param b de type NB
 * @return un NB de valeur équivalente à (1 / b) % a
 *
 * Retourne l'inverse de a modulo b calculé avec l'algorithme d'Euclide.
 */
NB euclideNBfct(NB a, NB b) {
	NB r = a, u = INTtoNB(1), v = INTtoNB(0);
	NB r_ = b, u_ = INTtoNB(0), v_ = INTtoNB(1);
	NB r_temp, u_temp, v_temp, q;
	while (!isEqual(r_, INTtoNB(0))) {
		q = quoNBbyNBfct(r, r_);
		r_temp = r;
		u_temp = u;
		v_temp = v;
		r = r_;
		u = u_;
		v = v_;
		r_ = substmultNBfct(r_temp, q, r_);
		u_ = substmultNBfct(u_temp, q, u_);
		v_ = substmultNBfct(v_temp, q, v_);
	}
	return v;
}

/**
 * \fct Cles genereCles()
 * @return une structure Cles contenant un couple de cles
 *
 * Retourne un couple clé publique / clé privée associée.
 */
Cles genereCles() {
	Cles c;
	NB p, q, n, phi, e, d, t;
	printf("Generation des cles ");
	p = NBAleaPremier(5);
	q = NBAleaPremier(1);
	putchar('.');

	//afficheln("p = ", p);
	//afficheln("q = ", q);

	n = multNBbyNBfct(p, q);
	p = substNBfct(p, INTtoNB(1));
	q = substNBfct(q, INTtoNB(1));
	phi = multNBbyNBfct(p, q);
	putchar('.');

	do {
		e = NBAlea(phi);
		t = pgcdNBfct(phi, e);
	} while (!isEqual(t, INTtoNB(1)));
	putchar('.');

	d = euclideNBfct(phi, e);
	while (d.signe == -1)
		d = sumNB(d, phi);
	printf(".\n");

	c.n = n;
	c.e = e;
	c.d = d;

	//affiche_cles("", c);

	//afficheln("Verification :", resNBbyNBfct(multNBbyNBfct(e, d), phi));

	return c;
}

