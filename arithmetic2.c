#include "rsa.h"

NB newNB() {
    NB n={.lg=0};
    int i;
    for(i=0; i<LG_MAX; i++) {
        n.nb[i]=0;
    }
    return n;
}

void affiche(NB n)
{
    for (int i=n.lg-1; i>=0; i--)
        printf("%x",n.nb[i]);
    printf(" %d fin \n",n.lg);
}

void mult(int taille, INT v, INT w, INT *msb, INT *lsb) {
    uint64_t res= (uint64_t)v*w, deuxP32=1;
    deuxP32=deuxP32<<(taille*8);
    //*lsb=(INT) (res%(long)pow(2,taille*8));
    *lsb=(INT) (res%deuxP32);
    *msb=(INT)(res>>taille*8);
    //printf(" %d %u %d %u %u \n", taille, v, w, *msb, *lsb);
}

void encode(INT v,NB *n,int p10)
{
    //printf(" %d %d\n",v,p10);
    INT msb,lsb;
    for (int i=0; i<n->lg; i++)
    {
        mult(sizeof(INT), n->nb[i], p10, &msb, &lsb );
        lsb+=v;
        v = msb+ (lsb<v) ;
        n->nb[i]=lsb;
    }
    if (v>0)
    {
        n->nb[n->lg]=v;
        (n->lg)++;
    }
}


void atoINT(char * str, NB* n)
{
    int taillePaquet[]= {-1,2,4,-1,9};
    int p10[]= {-1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,1000000000};
    int nbChiffres=taillePaquet[sizeof(INT)];
    INT nb=0;
    p10[0]=p10[nbChiffres];
    for (int i=1; i<=strlen(str); i++)
    {
        assert(str[i-1] >= '0' && str[i-1] <= '9');
        //printf("%c\n",str[i-1]);
        nb=nb*10+val(str[i-1]);
        if ( (i%nbChiffres==0)||(i==strlen(str)))
        {
            encode(nb,n,p10[i%nbChiffres]);
            nb=0;
        }
    }
}

void sum(int taille, INT v, INT w, int *msb, INT *lsb) {
    uint64_t res= (uint64_t)v+w, deuxP32=1;
    deuxP32=deuxP32<<(taille*8);
    //*lsb=(INT) (res%(long)pow(2,taille*8));
    *lsb=(INT) (res%deuxP32);
    *msb=(int)(res>>taille*8);
    //printf(" %d %u %d %u %u \n", taille, v, w, *msb, *lsb);
}

void sumNB(NB a, NB b, NB *res) {
    int i, msb;
    INT lsb;
    res->nb[0]=(INT)0;
    for(i=0; (i<a.lg)||(i<b.lg); i++) {
        sum(sizeof(INT), a.nb[i], b.nb[i], &msb, &lsb);
        res->nb[i]+=lsb;
        (res->lg)++;
        if(msb!=0) {
            res->nb[i+1]=msb;
            (res->lg)++;
        }
    }
}

