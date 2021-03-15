# Description

Ce programme met en oeuvre un simple client et serveur TCP qui ont la 
possibilité de s'envoyer des fichiers. Le client écoute sur une adresse 
et un port. Le client a la posibilité de se connecter au serveur et d'envoyer 
des requêtes de lecture ou d'écriture au serveur.

Les commandes sont envoyés sur une connexion TCP et les données sont envoyées 
sur une autre connexion TCP. Avec l'option -f du coté du client il est possible 
d'avoir une version rapide  du client et du serveur. 
Sans l'option -f on a une version très lente  du client et du serveur. 

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

` gbeffasarl.py` est un script Mininet qui met en oeuvre la topologie réseau
suivante. Le lien r0->r1 est un goulot d'étranglement. Les clients c1, c2 et c3
sont d'un côté du goulot d'étranglement et les serveur s1, s2 et s3 sont de 
l'autre coté du goulot d'étranglement.

```
              c3    s1    
               |    |
               |    |
         c1----r0--r1----s2
               |    | 
               |    | 
               |    | 
              c2    s3  
               
``` 


```
  $ sudo python3 gbeffasarl.py <delai> <bw> <loss_rate> <jitter>
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

`bw` est la bande passante. Une valeur possible peut être 100 soit 100Mbps.

`delay` est le délai, c'est à dire le temps de propagation d'un signal de r0 à
r1. Une valeur possible est 10, soit 10 ms pour simuler un LAN.

`loss_rate` est le taux de perte sur le lien en %. Une valeur possible est 0.

`jitter` est la variaance du délai. Une valeur possible est 0.

```
  $ sudo python3 gbeffasarl.py <delay> <bw> <loss_rate> <jitter>
```

Vous pouvez lancer les machines c1 et s1 à l'aide de la commande suivante:

```
  mininet> xterm c1 s1
``` 

s1 étant le serveur et c1 le client vous pouvez exécuter les commandes suivantes sur chacun d'eux.

Sur s1 

```
  $ ./server -s 192.168.3.100 -p 4444 -P 4445 -S send_log_file_path -R recv_log_file_path
``` 

sur c1 pour la version lente
```
  $ ./client -s 192.168.3.100 -p 4444 -P 4445 -S send_log_file_path -R recv_log_file_path -i src_file -o dst_file 
```

Sur c1 pour la version rapide
```
  $ ./client -s 192.168.3.100 -p 4444 -P 4445 -S send_log_file_path -R recv_log_file_path -i src_file -o dst_file -f
```

Pour lancer plusieurs clients à la fois vous pouvez utiliser le script test.sh

```
  ./test.sh 192.168.3.100 4444 4445 <fichier_a_transferer>
```

# Bugs
Pour les éventuels problèmes veuillez contacter assogba.emery@gmail.com
