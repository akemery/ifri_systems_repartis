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
## Tester le serveur et le client
### Côté du serveur
```
  $ ./server -s server_addr -p server_port -P server_port2 -S send_log_file_path -R recv_log_file_path
```

### Côté du client

```
  $ ./client -s server_addr -p server_port -P port2 -S send_log_file_path -R recv_log_file_path -i src_file -o dst_file [-f]
```

# Test avec mininet
Pour tester avec mininet vous devez au préalable installer mininet. Pour savoir
comment installer Mininet veuillez visiter le site de Mininet http://mininet.org/download/.

## Lancer le script python

```
  $ sudo python3 gbeffasarl.py
```

Il est possible d'avoir une erreur du controlleur.

```
tcp        0      0 0.0.0.0:6653            0.0.0.0:*               LISTEN     
2664/controller
```

Pour résoudre cette erreur il faut saisir la commande suivante:

```
 sudo fuser -k 6653/tcp
```

Puis relancer le script python
```
  $ sudo python3 gbeffasarl.py
```

` gbeffasarl.py` est un script Mininet qui met en oeuvre la topologie réseau
suivante.

```
         h2
         |
   h1---r0 -- r1--h3
               |
               h4
               
``` 
Vous pouvez lancer les machine h1 et h2 à l'aide de la commande suivante:

```
  mininet> xterm h1 h2
``` 

En supposant que h1 est le client et h2 est le serveur. 

Sur h2 

```
  ./server 192.168.3.100 4444 
```

Sur h1
```
  ./client 192.168.3.100 4444 <chemin_fichier_src> <chemin_fichier_dest>
```

# Bugs
Pour les éventuels problèmes veuillez contacter assogba.emery@gmail.com
