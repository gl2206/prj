# Projet NF05 : le chiffrement RSA

## Introduction

Le but de ce projet est d’implémenter en langage C un programme capable de générer des clés de chiffrement et de déchiffrement basées sur le système de cryptage RSA, et d’utiliser ces clés pour chiffrer et déchiffrer des fichiers sans perte ni modification de ces derniers. 

Le [chiffrement RSA](https://fr.wikipedia.org/wiki/Chiffrement_RSA) est un système de cryptage asymétrique mettant en oeuvre un couple de clés : la première sert au chiffrement et la deuxième au déchiffrement.
Ce projet utilise bien sûr [l'algorithme de Miller/Rabin](https://fr.wikipedia.org/wiki/Test_de_primalit%C3%A9_de_Miller-Rabin) pour déterminer la primalité des entiers ainsi que [l'algorithme d'Euclide étendu](https://fr.wikipedia.org/wiki/Algorithme_d%27Euclide_%C3%A9tendu) pour le calcul des clés.
L'implémentation a été réalisée en C (sans utiliser de librairies spécifiques comme la librairie de calcul en précision infinie [GNU MP](https://fr.wikipedia.org/wiki/GNU_MP))


## détails d'implémentation

### Encodage des NB via une chaîne de caractères
Lors des phases de développement et de test, il s’est avéré nécessaire de pouvoir affecter une valeur à une structure NB. Cependant, aucun type prédéfini du langage C n’est capable de stocker des nombres de très grandes tailles. Ainsi a été développé une fonction équivalente à atoi() de la librairie stdlib, prenant une chaîne de caractère contenant un nombre en base 10 et retournant un NB de même valeur : la fonction atoNB().

Fonctionnement général : la fonction atoNB() coupe le nombre à encoder en paquets de plusieurs chiffres (2, 4, ou 9 chiffres suivant le type d’entier utilisé) et range chaque paquet un par un dans un NB. Le découpage s’effectue de la gauche vers la droite (c.à.d à partir des chiffres les plus importants). Chaque paquet est ensuite encodé dans une fonction à part. 

Encodage : pour encoder un paquet dans le NB, il est nécessaire de multiplier ce dernier par une puissance de 10 correspondant au nombre de chiffres du paquet. Cela permet ainsi de laisser de la place au paquet qui est alors ajouté au NB en même temps que la multiplication.
### NB signés
Lors de l’implémentation de la fonction effectuant l’algorithme d’Euclide étendu, il fût établi que les entiers devait être signés afin de permettre le fonctionnement correct du programme. N’ayant pas trouvé d’alternative utilisant des entiers non signés, il fût nécessaire d’ajouter l’entier signe dans la structure NB et de modifier les fonctions arithmétiques de base (somme, soustraction, multiplication, division et modulo) afin de prendre en compte le signe des NB lors des opérations. Une fonction somme et une fonction soustraction ont alors été implémentées et utilisées pour réaliser l’algorithme d’Euclide étendu.
### La librairie GMP
Lors du développement de l’algorithme général permettant la génération des clés, un programme de test a été écrit à l’aide de la librairie GMP. Ce dernier a permis de travailler en même temps sur le fonctionnement général de la génération des clés et sur la construction de toutes les opérations de calcul pour les NB. Cela a permis un gain de temps lors de l’implémentation de cet algorithme dans le programme final, les fonctions associées étant à ce stade fonctionnelles.
### Lecture / Écriture de blocs
Pour optimiser au mieux le chiffrage et le déchiffrage d’un fichier, il faut pouvoir lire ce dernier par blocs de taille approchant celle de n, c.à.d plusieurs centaines de bits. Cependant, la taille du bloc n’est pas forcément égale à un certain nombre d’octets. Ainsi, un bloc peut tout à fait commencer et / ou finir en plein milieu d’un octet. Cela pose un problème du fait qu’aucune fonction des librairies standards du langage C ne travaille sur des bits : en effet, les fonctions de lecture et d’écriture binaire comme fread() et fwrite() de la librairie stdio ne peuvent lire ou écrire qu’un certain nombre d’octets. Il est donc nécessaire d’implémenter une fonction de lecture et une fonction d’écriture capable de travailler bit à bit.
L’implémentation des fonctions de lecture et d’écriture bit à bit ont été réalisées à l’aide de variables statiques afin de conserver le numéro du bit à lire ou écrire ainsi que l’octet en cours de construction ou d’écriture à travers l’appel successif des fonctions. 
Ainsi, pour la lecture, on récupère octet par octet les valeurs stockées dans le fichier et on lit chaque bit un par un. De cette façon, on ne récupère que le nombre de bits souhaité, le reste étant stocké dans une variable statique qui sera utilisée pour le prochain bloc. Lorsqu’on arrive à la fin du fichier, le dernier bloc lu est complété avec des zéros s’il le faut et on fait remonter l’information via une variable passée par argument.
De même, pour l’écriture, chaque bit de l’octet en cours de construction est positionné séparément. Lorsque la fin du bloc est atteinte et qu’il s’agit du dernier bloc, on complète le dernier octet avec des zéros s’il le faut et on l’écrit dans le fichier.

## Principe de génération du couple de clés
La génération des clés de chiffrement et de déchiffrement se déroule de la manière suivante :

On choisit deux nombres p et q premiers distincts. Il est préférable de les prendre le plus grand possible pour garantir la sécurité du couple de clés généré.

On calcule n = p * q ainsi que φ(n), l’image de n par la fonction indicatrice d’Euler. Pour ce cas particulier où n n’admet que quatre diviseurs (1, p, q, n), on a φ(n) = (p - 1) * (q - 1).

On choisit un nombre e premier avec φ(n) et strictement inférieur à ce dernier.

On calcule alors d tel que (e * d) % φ(n) = 1 (c.à.d l’inverse de e modulo n). Il existe une infinité de nombres respectant cette condition, ainsi on privilégiera le choix du plus petit entier positif.

Le couple (n, e), appelé clé publique, permet le chiffrement d’un message et le couple (n, d), appelé clé privée, le déchiffrement de ce dernier. 

## Chiffrement et déchiffrement
Pour chiffrer un message (numérique), on s’assure d’abord que ce dernier est strictement inférieur à n ou alors on le découpe en bloc de taille inférieure au nombre de bits de n. On a alors :

Message chiffré = (Message à la puissance e) % n

On obtient donc un message chiffré toujours strictement inférieur à n.

Le déchiffrement d’un message crypté se déroule de la même façon en utilisant cette fois la clé privée :

Message déchiffré = ((Message chiffré) à la puissance d) % n

On obtient alors le message d’origine.

## Sécurité du système de cryptage RSA
La sécurité de ce système de cryptage repose uniquement sur le fait que les nombres p et q sont suffisamment grand, afin de ne pas pouvoir les retrouver à partir de n. En effet, si on réussit à trouver un des diviseurs de n différent de 1 et de lui-même, on peut alors réussir à calculer φ(n) et à retrouver la clé privée à partir de la clé publique.
