# ENSEA in the Shell (enseash)

Créer un micro-shell Linux. [cite_start]L'objectif était de comprendre comment fonctionnent les processus, les appels système et les redirections. [cite: 6, 12, 25]

# Question 1 : Message d'accueil et Prompt

- [cite_start]Nous avons commencé par afficher un message de bienvenue et un prompt simple `enseash %`. [cite: 27, 29, 31]
- [cite_start]On a utilisé la fonction `write` au lieu de `printf` car elle est plus adaptée à la programmation système et n'utilise pas de buffer. [cite: 19]

# Question 2 : Exécution de commandes simples

- [cite_start]Nous avons créé une boucle de lecture qui récupère la commande de l'utilisateur. [cite: 32, 33]
- [cite_start]On utilise `fork()` pour créer un processus fils et `execvp()` pour lancer la commande, tandis que le père attend avec `wait()`. [cite: 34, 35]

# Question 3 : Gestion de la sortie (exit et Ctrl+D)

- [cite_start]Nous avons ajouté la gestion de la commande `exit` pour quitter le shell proprement. [cite: 41, 43]
- [cite_start]Le shell s'arrête aussi si l'utilisateur fait `Ctrl+D`, en vérifiant si la fonction `read` renvoie 0. [cite: 41]

# Question 4 : Affichage du code de retour ou du signal

- [cite_start]Le prompt a été amélioré pour afficher le statut de la dernière commande, par exemple `[exit:0]` ou `[sign:9]`. [cite: 46, 48, 49]
- Pour cela, on utilise les macros `WIFEXITED` et `WTERMSIG` sur le statut récupéré par `wait()`.

# Question 5 : Mesure du temps d'exécution

- [cite_start]Nous avons intégré `clock_gettime` pour mesurer le temps exact mis par chaque commande. [cite: 50]
- [cite_start]Le temps est affiché en millisecondes dans le prompt, par exemple `[exit:0|10ms]`. [cite: 54, 55]

# Question 6 : Exécution de commandes avec arguments

- [cite_start]On a modifié la lecture pour découper la commande en plusieurs arguments à l'aide de `strtok()`. [cite: 56]
- [cite_start]Cela nous permet de lancer des commandes plus complexes comme `ls -l` ou `hostname -i`. [cite: 57, 59]

# Question 7 : Gestion des redirections (< et >)

- [cite_start]Nous avons ajouté la gestion des redirections de flux. [cite: 63]
- [cite_start]On cherche les symboles `>` ou `<` dans les arguments, puis on utilise `open()` et `dup2()` pour rediriger l'entrée ou la sortie vers un fichier. [cite: 64, 65]
