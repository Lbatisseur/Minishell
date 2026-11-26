# Minishell (École 42)

Un interpréteur de commandes Unix écrit en C, sur le modèle de `bash`.
Projet réalisé en binôme avec [Akyra888](https://github.com/Akyra888).

## Ce que fait le programme

- Affiche une invite, lit la commande (bibliothèque `readline`) et garde l'historique.
- Découpe la ligne en éléments (lexing), vérifie la syntaxe puis construit les commandes (parsing).
- Remplace les variables d'environnement (`$HOME`, `$?`) et gère les guillemets simples et doubles.
- Lance les programmes avec `fork` et `execve`, en cherchant l'exécutable dans le `PATH`.
- Enchaîne les commandes avec des pipes `|` et gère les redirections `<`, `>`, `>>` et le heredoc `<<`.
- Réagit à `Ctrl-C`, `Ctrl-D` et `Ctrl-\` comme `bash`.
- Commandes intégrées : `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.

## Utilisation

Sur macOS, installer d'abord `readline` avec Homebrew :

```sh
brew install readline
make
./minishell
```

## Organisation du code

| Dossier | Rôle |
| --- | --- |
| `parsing/` | Découpage de la ligne, syntaxe, heredoc, conversion en commandes |
| `exec/` | Exécution, pipes, redirections, commandes intégrées |
| `signal/` | Gestion des signaux |
| `init/` | Environnement et invite |
| `utils/` | Fonctions communes (chaînes, mémoire, affichage) |
| `libft/` | Ma bibliothèque C (premier projet de 42) |
