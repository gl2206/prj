
#include "rsa.h"

//void reverse(char *nb);
//char* add(char nb1[], char nb2[]);
//char* mult(char nb1[], char nb2[]);

int main()
{
    NB n1=newNB(), n2=newNB(), n3=newNB();
    char v1[50], v2[50];
    strcpy(v1, "1234512459987");
    strcpy(v2, "2345678901234");
    //INT n1[20],n2[20];
    atoINT(v1,&n1);
    atoINT(v2,&n2);
    affiche(n1);
    affiche(n2);
    sumNB(n1, n2, &n3);
    affiche(n3);
    /*
    uint64_t m=1;
    m=m<<32;
    printf("\n %lu ", m);*/
/*    reverse(v1);
    reverse(v2); */
//    affiche(add(v1, v2));
//    mult(v1, v2);
   return 0;
}
