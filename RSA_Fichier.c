/*
 * RSA_Fichier.c
 */
#include "RSA.h"

/**
 * \fct int lireBitFichier(FILE *f, int *err, int reset)
 * @param f de type *FILE
 * @param err de type *int
 * @param reset de type int (en usage normal, positionner � la valeur 0, sinon 1 pour r�initialiser les variables statiques)
 * @return un entier, le bit courant lu dans le fichier.
 *
 * Retourne le bit courant lu dans le fichier point� par f. Si on atteint la fin du fichier, on positionne err � 1.
 * En appelant la fonction avec reset positionn� � 1, le num�ro du bit courant est repositionn� � z�ro.
 */
int lireBitFichier(FILE *f, int *err, int reset) {
	static char c;
	static int numBit;
	if(reset) {
		numBit = 0;
		return 0;
	}
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
	return b;
}

/**
 * \fct NB lireBlockFichier(FILE *f, int nbBits, int *err)
 * @param f de type FILE*
 * @param nbBits de type int
 * @param err de type int* (positionn� � 1 par lireBitFichier lorsqu'on arrive � la fin du fichier f)
 * @return un NB contenant la valeur du bloc lu
 *
 * Retourne un block de nbBits bits lu dans le fichier point� par f. Si on atteint la fin du fichier, lireBitFichier positionne err � 1.
 */
NB lireBlockFichier(FILE *f, int nbBits, int *err) {
	NB result = newNB();
	int i;
	INT b;
	for (i = 0; i < nbBits; i++) {
		b = lireBitFichier(f, err, 0);
		if (*err == 1)
			break;
		decaleBitNB(&result, 1);
		result = sumNBfct(result, INTtoNB(b));
	}
	if (*err == 1)
		decaleBitNB(&result, nbBits - i);
	return result;
}

/**
 *
 * \fct void ecrireBlockFichier(FILE *f, NB v, int nbBits, int err, int reset)
 * @param f de type FILE*
 * @param v de type NB
 * @param nbBits de type int
 * @param err de type int
 * @param reset de type int
 *
 * Ecrit nbBits bits de la valeur de v dans le fichier point� par f. Si err est positionn� � 1, compl�te et �crit le dernier octet.
 * En appelant la fonction avec reset positionn� � 1, le num�ro du bit courant est repositionn� � z�ro.
 */
void ecrireBlockFichier(FILE *f, NB v, int nbBits, int err, int reset) {
	static int numBit;
	static char c;
	if(reset) {
		numBit = 0;
		return;
	}
	int i, b;
	for (i = 0; i < nbBits; i++) {
		b = getBitNB(v, nbBits - i - 1);
		if (b != 0)
			b = 1;
		c <<= 1;
		c += b;
		if (numBit < 7)
			numBit++;
		else {
			fputc(c, f);
			c = 0;
			numBit = 0;
		}
	}
	if(err && c != 0) {
		while(numBit < 8) {
			c <<= 1;
			numBit++;
		}
		fputc(c, f);
	}
	return;
}


/**
 * \fct void enregistreCles(Cles c, FILE *publique, FILE *privee)
 * @param c de type Cles
 * @param publique de type FILE*
 * @param privee de type FILE*
 *
 * Ecrit la cl� publique de c dans le fichier point� par publique et la cl� priv�e de c dans le fichier point� par privee.
 */
void enregistreCles(Cles c, FILE *publique, FILE *privee) {
	int i;
	long temp = 0;
	for (i = 0; i < c.n.lg; i++) {
		fwrite(c.n.nb + i, sizeof(INT), 1, publique);
		fwrite(c.n.nb + i, sizeof(INT), 1, privee);
	}
	fwrite(&temp, sizeof(long), 1, publique);
	fwrite(&temp, sizeof(long), 1, privee);
	for (i = 0; i < c.e.lg; i++)
		fwrite(c.e.nb + i, sizeof(INT), 1, publique);
	for (i = 0; i < c.d.lg; i++)
		fwrite(c.d.nb + i, sizeof(INT), 1, privee);
}

/**
 * \fct Cles lireClePublique(FILE *f)
 * @param f de type FILE*
 * @return une structure Cles contenant une cl� publique
 *
 * Lis une cl� publique dans le fichier point� par f et retourne cette cl� dans une structure Cles.
 *
 * Note : la cl� priv�e est mise � 0.
 */
Cles lireClePublique(FILE *f) {
	Cles c = { .d = newNB() };
	NB temp = newNB();
	int i = 0;
	do {
		fread(temp.nb + i, sizeof(INT), 1, f);
		temp.lg++;
		i++;
	} while (temp.nb[i - 1] != 0);
	temp.lg--;
	c.n = temp;
	fseek(f, sizeof(long) - sizeof(INT), SEEK_CUR);
	temp = newNB();
	i = 0;
	while (!feof(f)) {
		fread(temp.nb + i, sizeof(INT), 1, f);
		temp.lg++;
		i++;
	}
	temp.lg--;
	c.e = temp;
	return c;
}

/**
 * \fct Cles lireClePrivee(FILE *f)
 * @param f de type FILE*
 * @return une structure Cles contenant une cl� priv�e
 *
 * Lis une cl� priv�e dans le fichier point� par f et retourne cette cl� dans une structure Cles.
 *
 * Note : la cl� publique est mise � 0.
 */
Cles lireClePrivee(FILE *f) {
	Cles c = { .e = newNB() };
	NB temp = newNB();
	int i = 0;
	do {
		fread(temp.nb + i, sizeof(INT), 1, f);
		temp.lg++;
		i++;
	} while (temp.nb[i - 1] != 0);
	temp.lg--;
	c.n = temp;
	fseek(f, sizeof(long) - sizeof(INT), SEEK_CUR);
	temp = newNB();
	i = 0;
	while (!feof(f)) {
		fread(temp.nb + i, sizeof(INT), 1, f);
		temp.lg++;
		i++;
	}
	temp.lg--;
	c.d = temp;
	return c;
}

/**
 * \fct void encryptFile(Cles c, FILE *r, FILE *w)
 * @param c de type Cles
 * @param r de type FILE*
 * @param w de type FILE*
 *
 * Chiffre le contenu du fichier point� par r avec la cl� priv�e contenue dans c et le range dans le fichier point� par w.
 */
void encryptFile(Cles c, FILE *r, FILE *w) {
	NB buff;
	int err = 0, nbBits = nbBitNB(c.n);
	lireBitFichier(NULL, NULL, 1);
	ecrireBlockFichier(NULL, INTtoNB(0), 0, 0, 1);
	printf("Chiffrement du fichier   ");
	while (!err) {
		buff = lireBlockFichier(r, nbBits - 1, &err);
//		affiche("", buff);
		buff = expmodNBfct(buff, c.e, c.n);
//		afficheln(" -> ", buff);
		ecrireBlockFichier(w, buff, nbBits, err, 0);
		putchar('.');
	}
	putchar('\n');
}
/**
 * \fct void decryptFile(Cles c, FILE *r, FILE *w)
 * @param c de type Cles
 * @param r de type FILE*
 * @param w de type FILE*
 *
 * D�chiffre le contenu du fichier point� par r avec la cl� publique contenue dans c et le range dans le fichier point� par w.
 */
void decryptFile(Cles c, FILE *r, FILE *w) {
	NB buff;
	int err = 0, nbBits = nbBitNB(c.n);
	lireBitFichier(NULL, NULL, 1);
	ecrireBlockFichier(NULL, INTtoNB(0), 0, 0, 1);
	printf("Dechiffrement du fichier ");
	while (!err) {
		buff = lireBlockFichier(r, nbBits, &err);
//		affiche("", buff);
		buff = expmodNBfct(buff, c.d, c.n);
//		afficheln(" -> ", buff);
		ecrireBlockFichier(w, buff, nbBits - 1, err, 0);
		putchar('.');
	}
	putchar('\n');
}
