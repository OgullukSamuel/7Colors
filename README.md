La branche principale est le projet en C, la branche secondaire (python library) est en édition,

Le dossier suivant contient les éléments suivants:

-Le fichier "main" est un fichier de renvoie. Il permet à l'utilisateur de selectionner le type d'affichage et d'utilisation du 7color.

-Le fichier "Queue" contient toutes les fonctions relatives aux files utilisées, ainsi qu'une fonction de test inutilisée.

-Le fichier "console" permet de gérer les interactions consoles et de faire s'affronter deux ia entre elles, de tester le bon fonctionnement du programme, ou bien encore de faire un classement ELO d'agents.

-Le fichier "GameState" contient des fonctions relatives à la carte ainsi que le struct Gamestate qui contient la carte

-Le fichier "map_functions" contient diverses fonctions relatives au bon fonctionnement du jeu, notamment des fonctions afin d'obtenir des sous réseaux, d'obtenir les coups jouables par un agent ou bien pour savoir si une partie est finie ou non.

-Le fichier "utilities" contient diverses fonctions utilitaires utilisées pour faciliter le développement. Ceci contient entre autres les fonctions approximées de tanh et exp, ainsi qu'une fonction d'évaluation de complexité temporelle utilisée durant le développement.

-Le fichier "display" contient des fonctions relatives à l'affichage dans le terminal des informations. Il contient ainsi beaucoup d'interaction avec l'utilisateur. Il est surtout utilisé dans le mode console du jeu.

-Le fichier "SDL" contient tout le mode de jeu graphique ainsi que les interactions avec ce dernier. 



A ces fichiers s'ajoutent leurs entêtes, la police d'écriture Arial, utilisée pour les écritures dans le mode graphique, ainsi qu'un makefile. Ce makefile permet de créer un executable, appelé :"7color" qui contient le programme et ses deux modes d'utilisation.

Bon jeu !

![image](https://github.com/user-attachments/assets/5565e489-3eaa-4bb3-a5b2-ef01ebbe39c8)

![image](https://github.com/user-attachments/assets/2b826f8f-0df2-4289-a06f-c8edc65c5dbd)

