# ENSEA_TP_PROGSYS_2A
# ENSEA in the Shell (enseash) - Rapport d'avancement

Ce document détaille les choix d'implémentation et les fonctions système utilisées pour les questions 1 à 3 du TP.

## Question 1 : Message d'accueil et Prompt

**Objectif :** Afficher un message de bienvenue au démarrage et un prompt (`enseash %`) pour indiquer que le shell est prêt.

**Explication du code :**
Pour garantir un code propre ("clean code") et modifiable facilement, j'ai défini les messages (accueil, prompt, erreurs) via des macros (`#define`). Pour l'affichage, j'ai utilisé la fonction système `write` (plutôt que `printf`) car elle est non bufferisée et adaptée à la programmation système.

**Fonction utilisée :**

* **write** : `ssize_t write(int fd, const void *buf, size_t count);`
  * **Arguments :** Elle prend le descripteur de fichier (`1` pour la sortie standard `stdout`), le message à écrire, et la taille du message en octets.
  * **Retour :** Elle retourne le nombre d'octets qui ont été réellement écrits (ou -1 en cas d'erreur).

---

## Question 2 : Exécution des commandes (REPL)

**Objectif :** Lire la commande de l'utilisateur et l'exécuter. Le shell gère ici les commandes simples comme `fortune` et `date` (via la commande `date` ou des alias).

**Explication du code :**
Le shell utilise une boucle infinie (`while(1)`). Il lit l'entrée, supprime le retour à la ligne (`\n`), et compare la chaîne pour savoir quoi exécuter.
Pour lancer la commande sans quitter le shell, j'utilise le mécanisme Père/Fils :

1. `fork()` crée un processus fils.
2. Le fils exécute la commande avec `execl` et se termine.
3. Le père attend la fin du fils avec `wait()` avant de réafficher le prompt.

**Fonctions utilisées :**

* **read** : `ssize_t read(int fd, void *buf, size_t count);`
  * **Arguments :** Elle prend le descripteur (`0` pour l'entrée standard `stdin`), un buffer pour stocker le texte, et la taille max à lire.
  * **Retour :** Elle retourne le nombre d'octets lus (ce qui permet de placer le caractère de fin de chaîne `\0`).

* **strcmp** : `int strcmp(const char *s1, const char *s2);`
  * **Arguments :** Elle prend deux chaînes de caractères à comparer.
  * **Retour :** Elle retourne `0` si les chaînes sont strictement identiques.

* **fork** : `pid_t fork(void);`
  * **Arguments :** Aucun.
  * **Retour :** Retourne `0` au processus fils et le `PID` du fils au processus père.

* **execl** : `int execl(const char *path, const char *arg, ...);`
  * **Arguments :** Elle prend le chemin du programme (`/bin/date`), le nom de la commande, et les arguments (terminés par `NULL`).
  * **Retour :** Elle ne retourne rien si ça marche (car le programme courant est remplacé), ou -1 si ça échoue.

* **wait** : `pid_t wait(int *status);`
  * **Arguments :** Un pointeur vers un entier pour stocker le statut de fin.
  * **Retour :** Retourne le PID du fils qui vient de se terminer.

---

## Question 3 : Gestion de la sortie (Exit)

**Objectif :** Quitter le shell proprement soit avec la commande "exit", soit avec `Ctrl+D`.

**Explication du code :**
Avant d'exécuter une commande, je vérifie la saisie :

* **Commande "exit" :** Si `strcmp` détecte "exit", on affiche un message d'adieu et on sort de la boucle avec `break`.
* **Ctrl+D (EOF) :** Si `read` retourne `0` (ce qui correspond à `len == 0`), cela signifie "End Of File" (l'utilisateur a fait `Ctrl+D`). On affiche le message d'adieu et on quitte.