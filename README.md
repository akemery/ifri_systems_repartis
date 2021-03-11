# Description

Ce programme met en oeuvre un simple client et serveur TCP qui ont la 
possibilité de s'envoyer des fichiers. Le client écoute sur une adresse 
et un port. Le client a la posibilité de se connecter au serveur et d'envoyer 
des requêtes de lecture ou d'écriture au serveur.

# Compilation
  ```
    $ make
  ```
# Utilisation
## Côté du serveur
```
  $ ./server <addrip_du_serveur> <port> 
```

## Côté du client

```
  $ ./client <addrip_du_serveur> <port> <chemin_fichier_source> <chemin_fichier_dest>
```

# Bugs
Pour les éventuels problèmes veuillez contacter assogba.emery@gmail.com
