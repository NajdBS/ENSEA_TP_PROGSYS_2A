# ENSEA in the Shell (enseash)

Créer un micro-shell Linux .BL'objectif était de comprendre comment fonctionnent les processus, les appels système et les redirections.

# Question 1 : Message d'accueil et Prompt

- [cite_start]Nous avons commencé par afficher un message de bienvenue et un prompt simple `enseash %`.
- [cite_start]On a utilisé la fonction `write` au lieu de `printf` car elle est plus adaptée à la programmation système et n'utilise pas de buffer. 

# Question 2 : Exécution de commandes simples

- [cite_start]Nous avons créé une boucle de lecture qui récupère la commande de l'utilisateur. 
- [cite_start]On utilise `fork()` pour créer un processus fils et `execvp()` pour lancer la commande, tandis que le père attend avec `wait()`. 
# Question 3 : Gestion de la sortie (exit et Ctrl+D)

- [cite_start]Nous avons ajouté la gestion de la commande `exit` pour quitter le shell proprement.
- [cite_start]Le shell s'arrête aussi si l'utilisateur fait `Ctrl+D`, en vérifiant si la fonction `read` renvoie 0. 

# Question 4 : Affichage du code de retour ou du signal

- [cite_start]Le prompt a été amélioré pour afficher le statut de la dernière commande, par exemple `[exit:0]` ou `[sign:9]`. 
- Pour cela, on utilise les macros `WIFEXITED` et `WTERMSIG` sur le statut récupéré par `wait()`.

# Question 5 : Mesure du temps d'exécution

- [cite_start]Nous avons intégré `clock_gettime` pour mesurer le temps exact mis par chaque commande. 
- [cite_start]Le temps est affiché en millisecondes dans le prompt, par exemple `[exit:0|10ms]`. 

# Question 6 : Exécution de commandes avec arguments

- [cite_start]On a modifié la lecture pour découper la commande en plusieurs arguments à l'aide de `strtok()`. 
- [cite_start]Cela nous permet de lancer des commandes plus complexes comme `ls -l` ou `hostname -i`. 

# Question 7 : Gestion des redirections (< et >)

- [cite_start]Nous avons ajouté la gestion des redirections de flux. 
- [cite_start]On cherche les symboles `>` ou `<` dans les arguments, puis on utilise `open()` et `dup2()` pour rediriger l'entrée ou la sortie vers un fichier. 
