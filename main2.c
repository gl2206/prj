#include "rsa.h"

#define FCT 1

NB atoNBfct(char *str);
NB sumNBfct(NB a, NB b);
NB multNBbyNBfct(NB a, NB b);

int main2() {
	NB n1 = newNB(), n2 = newNB(), n3 = newNB();
	char v1[50], v2[50];
	strcpy(v1, "1234512459987");  // 11f6ead38d3
	strcpy(v2, "2345678901234");  // 222255b8ff2
//    strcpy(v1, "12345");  // 11f6ead38d3
//    strcpy(v2, "10");  // 222255b8ff2
	//INT n1[20],n2[20];
	if(FCT) {
		n1 = atoNBfct(v1);
		n2 = atoNBfct(v2);
	}
	else {
		atoNB(v1, &n1);
		atoNB(v2, &n2);
	}
	//affiche(n1);
	//affiche(n2);
	n1 = sumNBfct(n1, n2);
	affiche(n1); // 3419408C8C5
	if(FCT) multNBbyNB(n1, n2, &n3);
	else n3 = multNBbyNBfct(n1, n2);
	affiche(n3);
	/*
	 uint64_t m=1;
	 m=m<<32;
	 printf("\n %lu ", m);*/
/*   reverse(v1);
	 reverse(v2); */
//    affiche(add(v1, v2));
//    mult(v1, v2);
	return 0;
}
