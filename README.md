# TP PROG SYS (enseash)

Créer un micro-shell Linux . L'objectif était de comprendre comment fonctionnent les processus, les appels système et les redirections.

- **Question 1 : Message d'accueil et Prompt**
  
Nous avons commencé par afficher un message de bienvenue et un prompt simple `enseash %`.
On a utilisé la fonction `write` au lieu de `printf` car elle est plus adaptée à la programmation système et c'est ce qui est demandé dans l'énoncé du TP.

- **Question 2 : Exécution de commandes simples**
  
Nous avons créé une boucle de lecture qui récupère la commande de l'utilisateur.
On utilise `fork()` pour créer un processus fils et `execvp()` pour lancer la commande, tandis que le père attend avec `wait()`.

- **Question 3 : Gestion de la sortie (exit et Ctrl+D)**
  
Nous avons ajouté la gestion de la commande `exit` pour quitter le shell proprement.
Le shell s'arrête aussi si l'utilisateur fait `Ctrl+D`, en vérifiant si la fonction `read` renvoie 0.

- **Question 4 : Affichage du code de retour ou du signal**
  
Le prompt a été amélioré pour afficher le statut de la derniere commande, par exemple `[exit:0]` ou `[sign:9]`.
Pour cela, on utilise les macros `WIFEXITED` et `WTERMSIG` sur le statut récupéré par `wait()`.

- **Question 5 : Mesure du temps d'exécution**
  
Nous avons intégré `clock_gettime` pour mesurer le temps exact mis par chaque commande.
Le temps est affiché en millisecondes dans le prompt, par exmple `[exit:0|10ms]`.

- **Question 6 : Exécution de commandes avec arguments**
  
On a modifié la lecture pour découper la commande en plusieurs arguments à l'aide de `strtok()`.
Cela nous permet de lancer des commandes plus complexes comme `ls -l` ou `hostname -i`.

- **Question 7 : Gestion des redirections (< et >)**
  
Nous avons ajouté la gestion des redirections de flux.
On cherche les symboles `>` ou `<` dans les arguments, puis on utilise `open()` et `dup2()` pour rediriger l'entrée ou la sortie vers un fichier.

- ****
  
Nous avons réussi à terminer le TP jusqu'à la question 7. Les questios suivantes n'ont pas été traitées par manque de temps.
